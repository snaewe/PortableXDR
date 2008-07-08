AC_INIT(portablexdr, 4.0.10)
AM_INIT_AUTOMAKE

AC_CONFIG_HEADERS([config.h])

AC_CANONICAL_HOST
AC_PROG_CC
AC_CHECK_PROGS(AR, ar)
AC_PROG_INSTALL
AC_PROG_LIBTOOL

MINGW_EXTRA_LDFLAGS=
case "$host" in
 *-*-mingw*)
   MINGW_EXTRA_LDFLAGS="-no-undefined"
   ;;
esac
AC_SUBST(MINGW_EXTRA_LDFLAGS)

AC_CHECK_HEADERS([arpa/inet.h])
AC_CHECK_FUNCS([ntohl htonl ntohs htons])

AC_OUTPUT(Makefile)

