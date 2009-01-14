# PortableXDR - a free, portable XDR implementation.
# Copyright (C) 2008-2009 Red Hat Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

SUBDIRS = lib
ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = m4/gnulib-cache.m4

# Header files, located in $includedir/portablexdr-5/rpc so that
# they don't conflict with existing SunRPC header files.

nobase_include_HEADERS = \
	portablexdr-5/rpc/rpc.h \
	portablexdr-5/rpc/types.h \
	portablexdr-5/rpc/xdr_internal.h \
	portablexdr-5/rpc/xdr.h

# The library.

lib_LTLIBRARIES = libportablexdr.la
libportablexdr_la_SOURCES = \
	$(nobase_include_HEADERS)
libportablexdr_la_CFLAGS = -Wall -Werror
libportablexdr_la_LDFLAGS = @MINGW_EXTRA_LDFLAGS@

# Replacement 'rpcgen', named portable-rpcgen to avoid any
# conflicts with existing Sun rpcgen.

BUILT_SOURCES = rpcgen_parse.h
AM_YFLAGS = -d
bin_PROGRAMS = portable-rpcgen
portable_rpcgen_SOURCES = \
	rpcgen_int.h \
	rpcgen_scan.l \
	rpcgen_parse.y \
	rpcgen_ast.c \
	rpcgen_codegen.c \
	rpcgen_main.c
portable_rpcgen_CFLAGS = -Wall
#portable_rpcgen_CFLAGS += -DYYDEBUG
