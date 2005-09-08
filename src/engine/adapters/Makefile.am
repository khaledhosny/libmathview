
NULL =

noinst_LTLIBRARIES = libadapters.la

libadapters_la_CPPFLAGS = -DGMV_MathView_DLL
libadapters_la_SOURCES = \
  BoxMLMathMLAdapter.cc \
  MathMLBoxMLAdapter.cc \
  $(NULL)

noinst_HEADERS = \
  BoxMLMathMLAdapter.hh \
  MathMLBoxMLAdapter.hh \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/backend/common \
  $(NULL)

