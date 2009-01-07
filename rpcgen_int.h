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

#ifndef RPCGEN_INT_H
#define RPCGEN_INT_H

extern char *input_filename;

/* Global functions used by the scanner. */
extern void start_string (void);
extern char *end_string (void);
extern void add_char (int);
extern void add_string (const char *);

/* These functions print an error and then exit. */
extern void error (const char *, ...)
  __attribute__((noreturn, format(printf,1,2)));
extern void perrorf (const char *, ...)
  __attribute__((noreturn, format(printf,1,2)));

#endif /* RPCGEN_INT_H */
