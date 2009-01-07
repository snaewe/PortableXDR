/* -*- C -*-
 * rpcgen - Generate XDR bindings automatically.
 * Copyright (C) 2008 Red Hat Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>

#include "rpcgen_int.h"

static void print_version (void);
static void usage (const char *progname);
static void do_rpcgen (const char *filename);
static char *make_cpp_command (const char *filename);

/* Symbols exported from the scanner. */
extern FILE *yyin, *yyout;
extern int yyparse (void);
extern int yylineno;
extern int yydebug;

int
main (int argc, char *argv[])
{
  int opt;

  /* To enable debugging in the parser, you also need to compile
   * with -DYYDEBUG
   */
#if 0
  yydebug = 1;
#endif

  /* Note on command line arguments: We only support a small subset
   * of command line arguments, because of the reduced functionality
   * available in this version of rpcgen.  However we accept the
   * command line parameters from both GNU rpcgen and BSD rpcgen
   * and print appropriate errors for any we don't understand.
   */
  while ((opt = getopt (argc, argv, "AD:IK:LMSTVchlmno:s:t")) != -1) {
    switch (opt)
      {
	/*-- Options supported by any rpcgen that we don't support. --*/
      case 'D': case 'T': case 'K': case 'l': case 'm': case 't':
      case 's':
	error ("option '%c' is not supported by this PortableXDR rpcgen.\nYou may need to use an alternative rpcgen program instead.", opt);

	/*-- Options supported only by GNU rpcgen that we don't support. --*/
      case 'I': case 'n':
	error ("option '%c' is not supported by this PortableXDR rpcgen.\nIf you were expecting to use GNU rpcgen, try /usr/bin/rpcgen on a GNU host.", opt);

	/*-- Options supported only by BSD rpcgen that we don't support. --*/
      case 'A': case 'M': case 'L': case 'S':
	error ("option '%c' is not supported by this PortableXDR rpcgen.\nIf you were expecting to use BSD rpcgen, try /usr/bin/rpcgen on a BSD host.", opt);

	/*-- Options that we do support. --*/
      case 'c':
      case 'h':
      case 'o':
	;

	/* None of the other versions of rpcgen support a way to print
	 * the version number, which is extremely annoying because
	 * there are so many different variations of rpcgen around.
	 * So this option at least should be useful!
	 */
      case 'V':
	print_version ();
        exit (0);

	/*-- Usage case. --*/
      default:
	usage (argv[0]);
	exit (1);
      }
  }

  if (optind >= argc)
    error ("expected name of input file after options");

  while (optind < argc)
    do_rpcgen (argv[optind++]);

  exit (0);
}

static void
print_version (void)
{
  printf ("PortableXDR rpcgen %s\n", PACKAGE_VERSION);
}

static void
usage (const char *progname)
{
  print_version ();
  printf
    ("Generate XDR bindings automatically.\n"
     "\n"
     "Usage:\n"
     "  portable-rpcgen infile.x\n"
     "  portable-rpcgen -c|-h [-o outfile] infile.x\n"
     "  portable-rpcgen -V\n"
     "\n"
     "Options:\n"
     "  -c     Generate C output file only.\n"
     "  -h     Generate header output file only.\n"
     "  -o     Name of output file (normally it is 'infile.[ch]').\n"
     "  -V     Print the version and exit.\n"
     "\n"
     "In the first form, without -c or -h, we generate both output files.\n"
     "\n"
     "You can also list more than one input file on the command line, in\n"
     "which case each input file is processed separately.\n"
     "\n"
     );
  exit (0);
}

/* Called for each input file. */
static void
do_rpcgen (const char *filename)
{
  char *cmd;
  int r;

  free (input_filename);
  input_filename = NULL;

  cmd = make_cpp_command (filename);

  yyin = popen (cmd, "r");
  if (yyin == NULL)
    perrorf ("%s", cmd);
  free (cmd);

  yyout = stdout;

  /* Parse the input file.  This either succeeds or exits with an error. */
  r = yyparse ();
  pclose (yyin);

  if (r == 1)
    error ("parsing failed, file is not a valid rpcgen input");
  else if (r == 2)
    error ("parsing failed because we ran out of memory");

  free (input_filename);
  input_filename = NULL;
}

/* Concatenate $EXTCPP and filename, and make sure the filename is
 * quoted correctly.  Tedious.
 */
static char *
make_cpp_command (const char *filename)
{
  static const char good[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-.";
#define is_good(c) (strchr (good, (c)) != NULL)
  const char *p;

  /* We can use start_string etc. because this function is only used
   * outside the scanner.
   */
  start_string ();
  add_string (EXTCPP);
  add_char (' ');

  for (p = filename; *p; p++) {
    if (is_good (*p)) add_char (*p);
    else {
      add_char ('\\');
      add_char (*p);
    }
  }

  return end_string ();
}

/* Some fairly random functions which are used by the scanner for
 * constructing strings, reporting errors, etc.
 */

/* The scanner sets this to the name of the input file (from cpp)
 * if known.
 */
char *input_filename = NULL;

static char *str = NULL;
static int str_used, str_alloc;

void
start_string (void)
{
  if (str != NULL)
    error ("scanner called start_string without calling end_string");

  str_alloc = 128;
  str_used = 0;
  str = malloc (str_alloc);
  if (!str) perrorf ("malloc");
}

char *
end_string (void)
{
  char *s;

  if (str == NULL)
    error ("scanner called end_string without calling start_string");

  s = realloc (str, str_used+1);
  if (!s) perrorf ("realloc");

  str = NULL;
  s[str_used] = '\0';
  return s;
}

void
add_char (int c)
{
  str_used++;
  while (str_used >= str_alloc) {
    str_alloc <<= 1;
    str = realloc (str, str_alloc);
    if (!str) perrorf ("realloc");
  }
  str[str_used-1] = c;
}

void
add_string (const char *s)
{
  int i = str_used;
  int len = strlen (s);

  str_used += len;
  while (str_used >= str_alloc) {
    str_alloc <<= 1;
    str = realloc (str, str_alloc);
    if (!str) perrorf ("realloc");
  }
  memcpy (str+i, s, len);
}

void
error (const char *fs, ...)
{
  va_list arg;

  if (input_filename == NULL)
    fputs (PACKAGE, stderr);
  else
    fprintf (stderr, "%s:%d", input_filename, yylineno);
  fputs (": ", stderr);

  va_start (arg, fs);
  vfprintf (stderr, fs, arg);
  va_end (arg);

  fputc ('\n', stderr);

  exit (1);
}

void
perrorf (const char *fs, ...)
{
  va_list arg;
  int e = errno;

  if (input_filename == NULL)
    fputs (PACKAGE, stderr);
  else
    fprintf (stderr, "%s:%d", input_filename, yylineno);
  fputs (": ", stderr);

  va_start (arg, fs);
  vfprintf (stderr, fs, arg);
  va_end (arg);

  fputs (": ", stderr);
  errno = e;
  perror (NULL);

  exit (1);
}
