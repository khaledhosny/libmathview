
NULL =

noinst_LTLIBRARIES = libgtk_areamodel.la

if COND_T1LIB
MAYBE_T1_S = \
  Gtk_T1GlyphArea.cc \
  $(NULL)
MAYBE_T1_H = \
  Gtk_T1GlyphArea.hh \
  $(NULL)
else
MAYBE_T1_S = $(NULL)
MAYBE_T1_H = $(NULL)
endif

libgtk_areamodel_la_SOURCES = \
  Gtk_RenderingContext.cc \
  Gtk_PangoLayoutArea.cc \
  Gtk_PangoLayoutLineArea.cc \
  Gtk_PangoGlyphArea.cc \
  Gtk_XftGlyphArea.cc \
  Gtk_ColorArea.cc \
  Gtk_BackgroundArea.cc \
  Gtk_InkArea.cc \
  Gtk_WrapperArea.cc \
  $(MAYBE_T1_S) \
  $(NULL)

noinst_HEADERS = \
  Gtk_AreaFactory.hh \
  Gtk_RenderingContext.hh \
  Gtk_PangoLayoutArea.hh \
  Gtk_PangoLayoutLineArea.hh \
  Gtk_PangoGlyphArea.hh \
  Gtk_XftGlyphArea.hh \
  Gtk_ColorArea.hh \
  Gtk_BackgroundArea.hh \
  Gtk_InkArea.hh \
  Gtk_WrapperArea.hh \
  $(MAYBE_T1_H) \
  $(NULL)

INCLUDES = \
  $(PANGOX_CFLAGS) \
  $(XFT_CFLAGS) \
  $(GTK_CFLAGS) \
  $(T1_CFLAGS) \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  $(NULL)

