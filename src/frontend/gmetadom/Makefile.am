
NULL =

noinst_LTLIBRARIES = libfrontend_gmetadom.la

libfrontend_gmetadom_la_CPPFLAGS = -DGMV_FrontEnd_DLL
libfrontend_gmetadom_la_LIBADD = \
  $(DOM_LIBS) \
  $(NULL)

libfrontend_gmetadom_la_SOURCES = \
  MathMLEntitiesTable.cc \
  MathMLEntitiesTable.hh \
  gmetadom.hh \
  gmetadom_Builder.cc \
  gmetadom_Builder.hh \
  gmetadom_Model.cc \
  gmetadom_Setup.cc \
  gmetadom_Setup.hh \
  $(NULL)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  gmetadom_Model.hh \
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
  $(DOM_CFLAGS) \
  $(NULL)

