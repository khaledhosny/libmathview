
GTK_CFLAGS = @GTK_CFLAGS@
GTK_LIBS = @GTK_LIBS@

DOM_CFLAGS = @DOM_CFLAGS@
DOM_LIBS = @DOM_LIBS@

T1_CFLAGS = @T1_CFLAGS@
T1_LIBS = @T1_LIBS@

lib_LTLIBRARIES = libwidget.la

libwidget_la_LIBADD = \
  -lstdc++ \
  $(GTK_LIBS) \
  $(DOM_LIBS)

libwidget_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libwidget_la_SOURCES = \
  gtkmathview.cc

pkginclude_HEADERS = \
  gtkmathview.h

INCLUDES = \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/basictypes/mathvariants \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/gtk_areamodel \
  -I$(top_srcdir)/src/formatter/mathml_graphic_device \
  -I$(top_srcdir)/src/formatter/gtk_mathml_graphic_device

