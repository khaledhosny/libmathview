
noinst_LTLIBRARIES = libfrontend_gmetadom.la

libfrontend_gmetadom_la_SOURCES = \
  gmetadom.cc \
  RefinementContext.cc \
  MathMLEntitiesTable.cc \
  gmetadomIterators.cc \
  gmetadom_Setup.cc \
  gmetadom_Builder.cc

noinst_HEADERS = \
  gmetadom.hh \
  AbstractRefinementContext.hh \
  RefinementContext.hh \
  MathMLEntitiesTable.hh \
  gmetadomIterators.hh \
  gmetadom_Setup.hh \
  gmetadom_Builder.hh

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/engine/adapters \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/formatter/areamodel 
