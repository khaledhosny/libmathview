
NULL =

lib_LTLIBRARIES = libmathview_frontend_gmetadom.la

libmathview_frontend_gmetadom_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_frontend_gmetadom_la_LIBADD = \
  $(DOM_LIBS) \
  $(top_builddir)/src/frontend/common/libfrontend_common.la \
  $(top_builddir)/src/libmathview.la \
  $(NULL)

libmathview_frontend_gmetadom_la_SOURCES = \
  MathMLEntitiesTable.cc \
  gmetadom_Model.cc \
  gmetadom_Setup.cc \
  gmetadom_Builder.cc \
  $(NULL)

noinst_HEADERS = \
  MathMLEntitiesTable.hh \
  gmetadom.hh \
  gmetadom_Model.hh \
  gmetadom_Setup.hh \
  gmetadom_Builder.hh \
  $(NULL)

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/engine/adapters \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  $(NULL)

