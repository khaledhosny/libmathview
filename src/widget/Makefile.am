
GTK_CFLAGS = @GTK_CFLAGS@
GTK_LIBS = @GTK_LIBS@

DOM_CFLAGS = @DOM_CFLAGS@
DOM_LIBS = @DOM_LIBS@

T1_CFLAGS = @T1_CFLAGS@
T1_LIBS = @T1_LIBS@

lib_LTLIBRARIES = libgtkmathview.la

libgtkmathview_la_LIBADD = \
  -lstdc++ \
  $(GTK_LIBS) \
  $(DOM_LIBS) \
  $(top_builddir)/src/engine/mathml/libmathml.la \
  $(top_builddir)/src/engine/boxml/libboxml.la \
  $(top_builddir)/src/engine/common/libengine_common.la \
  $(top_builddir)/src/formatter/gtk_mathml_graphic_device/libgtk_mathml_graphic_device.la \
  $(top_builddir)/src/formatter/mathml_graphic_device/libmathml_graphic_device.la \
  $(top_builddir)/src/formatter/gtk_boxml_graphic_device/libgtk_boxml_graphic_device.la \
  $(top_builddir)/src/formatter/boxml_graphic_device/libboxml_graphic_device.la \
  $(top_builddir)/src/formatter/gtk_areamodel/libgtk_areamodel.la \
  $(top_builddir)/src/formatter/areamodel/libareamodel.la \
  $(top_builddir)/src/formatter/basictypes/mathvariants/libmathvariants.la \
  $(top_builddir)/src/common/libcommon.la

libgtkmathview_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libgtkmathview_la_SOURCES = \
  gtkmathview.cc

pkginclude_HEADERS = \
  gtkmathview.h

INCLUDES = \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
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

