
lib_LTLIBRARIES =
if COND_CUSTOM_READER
if COND_LIBXML2_READER
lib_LTLIBRARIES += libgtkmathview_custom_reader.la
endif
endif

if COND_LIBXML2
lib_LTLIBRARIES += libgtkmathview_libxml2.la
endif

if COND_GMETADOM
lib_LTLIBRARIES += libgtkmathview_gmetadom.la
endif

libgtkmathview_custom_reader_la_LIBADD = \
  -lstdc++ \
  $(GTK_LIBS) \
  $(XML_LIBS) \
  $(top_builddir)/src/frontend/libxml2_reader/libmathview_frontend_libxml2_reader.la \
  $(top_builddir)/src/frontend/custom_reader/libmathview_frontend_custom_reader.la

libgtkmathview_libxml2_la_LIBADD = \
  -lstdc++ \
  $(GTK_LIBS) \
  $(XML_LIBS) \
  $(top_builddir)/src/frontend/libxml2/libmathview_frontend_libxml2.la

libgtkmathview_gmetadom_la_LIBADD = \
  -lstdc++ \
  $(GTK_LIBS) \
  $(DOM_LIBS) \
  $(top_builddir)/src/frontend/gmetadom/libmathview_frontend_gmetadom.la

libgtkmathview_custom_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libgtkmathview_libxml2_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libgtkmathview_gmetadom_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libgtkmathview_custom_reader_la_SOURCES = gtkmathview_custom_reader.cc
libgtkmathview_libxml2_la_SOURCES = gtkmathview_libxml2.cc
libgtkmathview_gmetadom_la_SOURCES = gtkmathview_gmetadom.cc

EXTRA_DIST = gtkmathview_common.cc

gtkdir = $(includedir)/$(PACKAGE)/gtk
gtk_HEADERS = \
  gtkmathview.h \
  gtkmathview_common.h \
  gtkmathview_custom_reader.h \
  gtkmathview_libxml2.h \
  gtkmathview_gmetadom.h

INCLUDES = \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/custom_reader \
  -I$(top_srcdir)/src/frontend/libxml2_reader \
  -I$(top_srcdir)/src/frontend/libxml2 \
  -I$(top_srcdir)/src/frontend/gmetadom \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/basictypes/mathvariants \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/gtk_areamodel \
  -I$(top_srcdir)/src/formatter/mathml_graphic_device \
  -I$(top_srcdir)/src/formatter/gtk_mathml_graphic_device \
  -I$(top_srcdir)/src/formatter/boxml_graphic_device \
  -I$(top_srcdir)/src/formatter/gtk_boxml_graphic_device

