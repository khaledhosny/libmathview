
NULL =

noinst_LTLIBRARIES = libmathview_backend_gtk.la

if COND_BOXML
MAYBE_BOXML_S = Gtk_BoxGraphicDevice.cc
MAYBE_BOXML_H = Gtk_BoxGraphicDevice.hh
else
MAYBE_BOXML_S = $(NULL)
MAYBE_BOXML_H = $(NULL)
endif

if COND_T1LIB
MAYBE_T1_S = \
  Gtk_T1ComputerModernShaper.cc \
  Gtk_t1lib_T1GlyphArea.cc \
  $(NULL)
MAYBE_T1_H = \
  Gtk_T1ComputerModernShaper.hh \
  Gtk_t1lib_T1GlyphArea.hh \
  $(NULL)
else
MAYBE_T1_S = $(NULL)
MAYBE_T1_H = $(NULL)
endif

libmathview_backend_gtk_la_SOURCES = \
  Gtk_AdobeShaper.cc \
  Gtk_AdobeShaper.hh \
  Gtk_AreaFactory.hh \
  Gtk_Backend.cc \
  Gtk_Backend.hh \
  Gtk_BackgroundArea.cc \
  Gtk_BackgroundArea.hh \
  Gtk_ColorArea.cc \
  Gtk_ColorArea.hh \
  Gtk_DefaultPangoShaper.cc \
  Gtk_DefaultPangoShaper.hh \
  Gtk_InkArea.cc \
  Gtk_InkArea.hh \
  Gtk_MathGraphicDevice.cc \
  Gtk_MathGraphicDevice.hh \
  Gtk_PangoFontManager.cc \
  Gtk_PangoFontManager.hh \
  Gtk_PangoGlyphArea.cc \
  Gtk_PangoGlyphArea.hh \
  Gtk_PangoLayoutArea.cc \
  Gtk_PangoLayoutArea.hh \
  Gtk_PangoLayoutLineArea.cc \
  Gtk_PangoLayoutLineArea.hh \
  Gtk_PangoShaper.cc \
  Gtk_PangoShaper.hh \
  Gtk_RenderingContext.cc \
  Gtk_RenderingContext.hh \
  Gtk_WrapperArea.cc \
  Gtk_WrapperArea.hh \
  Gtk_XftFontManager.cc \
  Gtk_XftFontManager.hh \
  Gtk_XftGlyphArea.cc \
  Gtk_XftGlyphArea.hh \
  $(MAYBE_BOXML_S) \
  $(MAYBE_BOXML_H) \
  $(MAYBE_T1_S) \
  $(MAYBE_T1_H) \
  $(NULL)

INCLUDES = \
  $(PANGOX_CFLAGS) \
  $(XFT_CFLAGS) \
  $(GTK_CFLAGS) \
  $(T1_CFLAGS) \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/backend/common \
  $(NULL)
