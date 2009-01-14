dnl PortableXDR - a free, portable XDR implementation.
dnl Copyright (C) 2008-2009 Red Hat Inc.
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

AC_INIT(portablexdr, 4.9.2)
AM_INIT_AUTOMAKE

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_HOST
AC_PROG_CC
gl_EARLY

AM_PROG_CC_C_O

AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_PROG_LEX
AC_PROG_YACC

gl_INIT

AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_OFF_T

AC_CHECK_PROGS(AR, ar)

dnl Look for an external 'cpp' program which can run on a file with any
dnl extension.  The normal CPP can only run on files with a '.c'
dnl extension, therefore we prefer to use /usr/bin/cpp if it exists.
AC_PATH_PROG([EXTCPP], [cpp], [no])
if test "x$EXTCPP" = "xno"; then
  AC_MSG_FAILURE([Cannot find a working 'cpp' (C preprocessor) program])
fi
AC_DEFINE_UNQUOTED([EXTCPP], ["$EXTCPP"],
	[The C preprocessor command (may include command line options).])

MINGW_EXTRA_LDFLAGS=
case "$host" in
 *-*-mingw*)
   MINGW_EXTRA_LDFLAGS="-no-undefined"
   ;;
esac
AC_SUBST(MINGW_EXTRA_LDFLAGS)

AC_CHECK_HEADERS([arpa/inet.h sys/param.h])
AC_CHECK_FUNCS([ntohl htonl ntohs htons])

AC_CONFIG_FILES([Makefile lib/Makefile])
AC_OUTPUT
