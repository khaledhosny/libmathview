
noinst_LTLIBRARIES = libfrontend_common.la

libfrontend_common_la_SOURCES = \
  Builder.cc

noinst_HEADERS = \
  Reader.hh \
  Builder.hh

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/formatter/areamodel

# DOM is needed because String is an alias for DOM::String :-(