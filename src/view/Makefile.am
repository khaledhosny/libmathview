
lib_LTLIBRARIES =

if COND_LIBXML2
lib_LTLIBRARIES += libmathview_libxml2.la
endif

libmathview_libxml2_la_LIBADD = \
  -lstdc++ \
  $(XML_LIBS) \
  $(top_builddir)/src/frontend/libxml2/libmathview_frontend_libxml2.la

libmathview_libxml2_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_libxml2_la_SOURCES = libxml2_MathView.cc

pkginclude_HEADERS = \
  libxml2_MathView.hh

INCLUDES = \
  $(XML_CFLAGS) \
  -I$(top_srcdir)/auto \
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
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/gtk_areamodel \
  -I$(top_srcdir)/src/formatter/mathml_graphic_device \
  -I$(top_srcdir)/src/formatter/gtk_mathml_graphic_device \
  -I$(top_srcdir)/src/formatter/boxml_graphic_device \
  -I$(top_srcdir)/src/formatter/gtk_boxml_graphic_device
