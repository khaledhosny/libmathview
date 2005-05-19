
NULL =

lib_LTLIBRARIES = libmathview_frontend_libxml2_reader.la

libmathview_frontend_libxml2_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_frontend_libxml2_reader_la_LIBADD = \
  $(XML_LIBS) \
  $(top_builddir)/src/frontend/common/libfrontend_common.la \
  $(top_builddir)/src/libmathview.la \
  $(NULL)

libmathview_frontend_libxml2_reader_la_SOURCES = \
  libxmlXmlReader.cc \
  libxml2_reader_Builder.cc \
  libxml2_reader_Setup.cc \
  $(NULL)

noinst_HEADERS = \
  libxmlXmlReader.hh \
  libxml2_reader_Model.hh \
  libxml2_reader_Builder.hh \
  libxml2_reader_Setup.hh \
  $(NULL)

INCLUDES = \
  $(XML_CFLAGS) \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/engine/adapters \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/backend/common \
  $(NULL)

