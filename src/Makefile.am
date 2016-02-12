
AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CXXFLAGS =\
  -Wall\
  -ggdb2 \
  -Wall \
  -Wextra \
  -std=c++11

lib_LTLIBRARIES = libspinnistate.la

libspinnistate_la_SOURCES = \
	state.cpp \
	state-machine.cpp \
	state-container.cpp

libspinnistate_la_LDFLAGS = 

libspinnistate_la_LIBADD = 

include_HEADERS = \
	libspinnistate.h \
	state-machine.h \
	state.h \
	state-container.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libspinnistate.pc

EXTRA_DIST = \
	libspinnistate.pc.in
