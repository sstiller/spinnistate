
AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	$(CHAISCRIPT_CFLAGS) \
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
	state-container.cpp \
	action-container.cpp \
	data-model.cpp \
	chai-script-data-model.cpp \
	transition.cpp \
	state-machine-element.cpp

libspinnistate_la_LDFLAGS = 

libspinnistate_la_LIBADD =  \
	$(CHAISCRIPT_LIBS)

include_HEADERS = \
	libspinnistate.h \
	list.h \
	hash-table.h \
	ordered-set.h \
	queue.h \
	state-machine.h \
	state.h \
	state-container.h \
	action-container.h \
	data-model.h \
	chai-script-data-model.h \
	transition.h \
	state-machine-element.h


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libspinnistate.pc

EXTRA_DIST = \
	libspinnistate.pc.in
