nobase_include_HEADERS = rpc/rpc.h rpc/types.h rpc/xdr.h

lib_LTLIBRARIES = libportablexdr.la
libportablexdr_la_SOURCES = xdr_array.c xdr_float.c xdr_mem.c xdr_reference.c \
	xdr.c xdr_intXX_t.c xdr_rec.c xdr_stdio.c \
	byteswap.h \
	rpc/rpc.h rpc/types.h rpc/xdr.h
libportablexdr_la_CFLAGS = -Wall -Werror -fno-strict-aliasing
libportablexdr_la_LDFLAGS = @MINGW_EXTRA_LDFLAGS@
