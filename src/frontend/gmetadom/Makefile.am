
noinst_LTLIBRARIES = libfrontend_gmetadom.la

libfrontend_gmetadom_la_SOURCES = \
  gmetadom_Reader.cc \
  gmetadom_Builder.cc

noinst_HEADERS = \
  gmetadom_Reader.hh \
  gmetadom_Builder.hh

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/formatter/areamodel 