
NULL =

noinst_LTLIBRARIES = libgtk_mathml_graphic_device.la

#libgtk_mathml_graphic_device_la_LIBADD = \
  #$(PANGOX_LIBS) \
  #$(GTK_LIBS) \
  #$(top_builddir)/src/formatter/basictypes/mathvariants/libmathvariants.la \
  #$(top_builddir)/src/formatter/areamodel/libareamodel.la \
  #$(top_builddir)/src/formatter/gtk_areamodel/libgtk_areamodel.la \
  #$(top_builddir)/src/formatter/mathml_graphic_device/libmathml_graphic_device.la

#libgtk_mathml_graphic_device_la_LDFLAGS = -version-info @AREAMODEL_VERSION_INFO@

if COND_T1LIB
MAYBE_T1_S = \
  T1FontManager.cc \
  Gtk_T1ComputerModernShaper.cc \
  $(NULL)
MAYBE_T1_H = \
  T1FontManager.hh \
  Gtk_T1ComputerModernShaper.hh \
  $(NULL)
else
MAYBE_T1_S = $(NULL)
MAYBE_T1_H = $(NULL)
endif

libgtk_mathml_graphic_device_la_SOURCES = \
  Gtk_MathGraphicDevice.cc \
  Gtk_PangoFontManager.cc \
  Gtk_XftFontManager.cc \
  Gtk_DefaultPangoShaper.cc \
  Gtk_PangoShaper.cc \
  Gtk_AdobeShaper.cc \
  Gtk_ComputerModernShaper.cc \
  $(MAYBE_T1_S) \
  $(NULL)

noinst_HEADERS = \
  Gtk_MathGraphicDevice.hh \
  Gtk_PangoFontManager.hh \
  Gtk_XftFontManager.hh \
  Gtk_DefaultPangoShaper.hh \
  Gtk_PangoShaper.hh \
  Gtk_AdobeShaper.hh \
  Gtk_ComputerModernShaper.hh \
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
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/gtk_areamodel \
  -I$(top_srcdir)/src/formatter/mathml_graphic_device
