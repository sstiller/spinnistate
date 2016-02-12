## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
  -std=c++11

AM_CFLAGS =\
  -Wall\
  -ggdb2 \
  -Wall \
  -Wextra

lib_LTLIBRARIES = libspinnistate.la

libspinnistate_la_SOURCES = \
	state-machine.cpp

libspinnistate_la_LDFLAGS = 

libspinnistate_la_LIBADD = 

include_HEADERS = \
	libspinnistate.h \
	state-machine.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libspinnistate.pc

EXTRA_DIST = \
	libspinnistate.pc.in
