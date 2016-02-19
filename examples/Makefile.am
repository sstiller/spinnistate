

bin_PROGRAMS =  \
	spinnitest

spinnitest_SOURCES =  \
	spinnitest.cpp

spinnitest_CXXFLAGS = \
  -Wall\
  -ggdb2 \
  -Wall \
  -Wextra \
  -std=c++11 \
  $(CHAISCRIPT_CFLAGS) 

spinnitest_LDFLAGS = \
  -L$(abs_builddir)/../src/

spinnitest_LDADD = \
  -lspinnistate \
  -lboost_system \
  $(CHAISCRIPT_LIBS) \
  -ldl 
