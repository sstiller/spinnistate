dnl Process this file with autoconf to produce a configure script.

AC_INIT(libspinnistate, 0.1)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

dnl AM_SILENT_RULES([yes])

AC_PROG_CC


AC_PROG_CPP
AC_PROG_CXX


AC_DISABLE_STATIC

LT_INIT


PKG_CHECK_MODULES(CHAISCRIPT,chaiscript)

AC_OUTPUT([
Makefile
src/libspinnistate.pc
src/Makefile
examples/Makefile
])
