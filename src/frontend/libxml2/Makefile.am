
noinst_LTLIBRARIES = libmathview_frontend_libxml2.la

libmathview_frontend_libxml2_la_SOURCES = \
  libxml2_Model.cc \
  libxml2_Setup.cc \
  libxml2_Builder.cc

noinst_HEADERS = \
  libxml2_Model.hh \
  libxml2_Setup.hh \
  libxml2_Builder.hh

INCLUDES = \
  $(XML_CFLAGS) \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/engine/adapters \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/formatter/areamodel 

