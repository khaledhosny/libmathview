
NULL =

lib_LTLIBRARIES =

if COND_CUSTOM_READER
lib_LTLIBRARIES += libmathview_custom_reader.la
endif

if COND_LIBXML2_READER
lib_LTLIBRARIES += libmathview_libxml2_reader.la
endif

if COND_LIBXML2
lib_LTLIBRARIES += libmathview_libxml2.la
endif

if COND_GMETADOM
lib_LTLIBRARIES += libmathview_gmetadom.la
endif

MATHVIEW_LIBS = \
  $(top_builddir)/src/backend/libbackend.la \
  $(top_builddir)/src/engine/libengine.la \
  $(top_builddir)/src/common/libcommon.la \
  $(NULL)

libmathview_custom_reader_la_LIBADD = \
  $(XML_LIBS) \
  $(top_builddir)/src/frontend/libxml2_reader/libmathview_frontend_libxml2_reader.la \
  $(top_builddir)/src/frontend/custom_reader/libmathview_frontend_custom_reader.la \
  $(MATHVIEW_LIBS) \
  $(NULL)

libmathview_libxml2_reader_la_LIBADD = \
  $(XML_LIBS) \
  $(top_builddir)/src/frontend/libxml2_reader/libmathview_frontend_libxml2_reader.la \
  $(MATHVIEW_LIBS) \
  $(NULL)

libmathview_libxml2_la_LIBADD = \
  $(XML_LIBS) \
  $(top_builddir)/src/frontend/libxml2/libmathview_frontend_libxml2.la \
  $(MATHVIEW_LIBS) \
  $(NULL)

libmathview_gmetadom_la_LIBADD = \
  $(DOM_LIBS) \
  $(top_builddir)/src/frontend/gmetadom/libmathview_frontend_gmetadom.la \
  $(MATHVIEW_LIBS) \
  $(NULL)

libmathview_custom_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libmathview_libxml2_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libmathview_libxml2_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libmathview_gmetadom_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_custom_reader_la_SOURCES = custom_reader_MathView.cc
libmathview_libxml2_reader_la_SOURCES = libxml2_reader_MathView.cc
libmathview_libxml2_la_SOURCES = libxml2_MathView.cc
libmathview_gmetadom_la_SOURCES = gmetadom_MathView.cc

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  custom_reader_MathView.hh \
  libxml2_reader_MathView.hh \
  libxml2_MathView.hh \
  gmetadom_MathView.hh

INCLUDES = \
  $(XML_CFLAGS) \
  $(DOM_CFLAGS) \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/custom_reader \
  -I$(top_srcdir)/src/frontend/libxml2_reader \
  -I$(top_srcdir)/src/frontend/libxml2 \
  -I$(top_srcdir)/src/frontend/gmetadom \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/backend/common \
  $(NULL)