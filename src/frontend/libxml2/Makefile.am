
NULL =

noinst_LTLIBRARIES = libfrontend_libxml2.la

libfrontend_libxml2_la_CPPFLAGS = -DGMV_FrontEnd_DLL
libfrontend_libxml2_la_LIBADD = \
  $(XML_LIBS) \
  $(NULL)

libfrontend_libxml2_la_SOURCES = \
  libxml2_Model.cc \
  libxml2_Setup.cc \
  libxml2_Builder.cc \
  libxml2_Setup.hh \
  libxml2_Builder.hh \
  $(NULL)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  libxml2_Model.hh \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/engine/adapters \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/backend/common \
  $(XML_CFLAGS) \
  $(NULL)

