
NULL =

if COND_CUSTOM_READER
MAYBE_CUSTOM_READER = libmathview_frontend_custom_reader.la
else
MAYBE_CUSTOM_READER = $(NULL)
endif

if COND_LIBXML2_READER
MAYBE_LIBXML2_READER = libmathview_frontend_libxml2_reader.la
else
MAYBE_LIBXML2_READER = $(NULL)
endif

if COND_LIBXML2
MAYBE_LIBXML2 = libmathview_frontend_libxml2.la
else
MAYBE_LIBXML2 = $(NULL)
endif

if COND_GMETADOM
MAYBE_GMETADOM = libmathview_frontend_gmetadom.la
else
MAYBE_GMETADOM = $(NULL)
endif

lib_LTLIBRARIES = $(MAYBE_CUSTOM_READER) $(MAYBE_LIBXML2_READER) $(MAYBE_LIBXML2) $(MAYBE_GMETADOM)

libmathview_frontend_custom_reader_la_LIBADD = \
  $(XML_LIBS) \
  $(top_builddir)/src/frontend/libxml2_reader/libfrontend_libxml2_reader.la \
  $(top_builddir)/src/frontend/custom_reader/libfrontend_custom_reader.la \
  $(top_builddir)/src/libmathview.la \
  -lstdc++ \
  $(NULL)

libmathview_frontend_libxml2_reader_la_LIBADD = \
  $(XML_LIBS) \
  $(top_builddir)/src/frontend/libxml2_reader/libfrontend_libxml2_reader.la \
  $(top_builddir)/src/libmathview.la \
  -lstdc++ \
  $(NULL)

libmathview_frontend_libxml2_la_LIBADD = \
  $(XML_LIBS) \
  $(top_builddir)/src/frontend/libxml2/libfrontend_libxml2.la \
  $(top_builddir)/src/libmathview.la \
  -lstdc++ \
  $(NULL)

libmathview_frontend_gmetadom_la_LIBADD = \
  $(DOM_LIBS) \
  $(top_builddir)/src/frontend/gmetadom/libfrontend_gmetadom.la \
  $(top_builddir)/src/libmathview.la \
  -lstdc++ \
  $(NULL)

libmathview_frontend_custom_reader_la_CPPFLAGS = -DGMV_FrontEnd_DLL
libmathview_frontend_libxml2_reader_la_CPPFLAGS = -DGMV_FrontEnd_DLL
libmathview_frontend_libxml2_la_CPPFLAGS = -DGMV_FrontEnd_DLL
libmathview_frontend_gmetadom_la_CPPFLAGS = -DGMV_FrontEnd_DLL

libmathview_frontend_custom_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined
libmathview_frontend_libxml2_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined
libmathview_frontend_libxml2_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined
libmathview_frontend_gmetadom_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined

libmathview_frontend_custom_reader_la_SOURCES = custom_reader_MathView.cc
libmathview_frontend_libxml2_reader_la_SOURCES = libxml2_reader_MathView.cc
libmathview_frontend_libxml2_la_SOURCES = libxml2_MathView.cc
libmathview_frontend_gmetadom_la_SOURCES = gmetadom_MathView.cc

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  Init.hh \
  custom_reader_MathView.hh \
  libxml2_reader_MathView.hh \
  libxml2_MathView.hh \
  gmetadom_MathView.hh \
  $(NULL)

INCLUDES = \
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
  $(XML_CFLAGS) \
  $(DOM_CFLAGS) \
  $(NULL)
