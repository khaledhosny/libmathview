
noinst_LTLIBRARIES = libgtk_mathml_graphic_device.la

#libgtk_mathml_graphic_device_la_LIBADD = \
  #$(PANGOX_LIBS) \
  #$(GTK_LIBS) \
  #$(top_builddir)/src/formatter/basictypes/mathvariants/libmathvariants.la \
  #$(top_builddir)/src/formatter/areamodel/libareamodel.la \
  #$(top_builddir)/src/formatter/gtk_areamodel/libgtk_areamodel.la \
  #$(top_builddir)/src/formatter/mathml_graphic_device/libmathml_graphic_device.la

#libgtk_mathml_graphic_device_la_LDFLAGS = -version-info @AREAMODEL_VERSION_INFO@

libgtk_mathml_graphic_device_la_SOURCES = \
  Gtk_MathGraphicDevice.cc \
  Gtk_PangoShaper.cc \
  Gtk_AdobeShaper.cc

noinst_HEADERS = \
  Gtk_MathGraphicDevice.hh \
  Gtk_PangoShaper.hh \
  Gtk_AdobeShaper.hh

INCLUDES = \
  $(PANGOX_CFLAGS) \
  $(XFT_CFLAGS) \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/basictypes/mathvariants \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/gtk_areamodel \
  -I$(top_srcdir)/src/formatter/mathml_graphic_device
