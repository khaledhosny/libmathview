
lib_LTLIBRARIES = libgtk_areamodel.la

libgtk_areamodel_la_LIBADD = \
  -lstdc++ \
  $(XFT_LIBS) \
  $(PANGOX_LIBS) \
  $(GTK_LIBS) \
  $(DOM_LIBS) \
  $(top_builddir)/src/formatter/areamodel/libareamodel.la

libgtk_areamodel_la_LDFLAGS = -version-info @AREAMODEL_VERSION_INFO@

libgtk_areamodel_la_SOURCES = \
  Gtk_RenderingContext.cc \
  Gtk_XftGlyphArea.cc \
  Gtk_PangoLayoutArea.cc \
  Gtk_PangoLayoutLineArea.cc \
  Gtk_PangoGlyphArea.cc \
  Gtk_ColorArea.cc \
  Gtk_InkArea.cc \
  Gtk_WrapperArea.cc

pkginclude_HEADERS = \
  Gtk_AreaFactory.hh \
  Gtk_RenderingContext.hh \
  Gtk_XftGlyphArea.hh \
  Gtk_PangoLayoutArea.hh \
  Gtk_PangoLayoutLineArea.hh \
  Gtk_PangoGlyphArea.hh \
  Gtk_ColorArea.hh \
  Gtk_InkArea.hh \
  Gtk_WrapperArea.hh

INCLUDES = \
  $(PANGOX_CFLAGS) \
  $(XFT_CFLAGS) \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/formatter/areamodel

