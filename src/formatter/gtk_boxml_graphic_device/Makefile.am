
noinst_LTLIBRARIES = libgtk_boxml_graphic_device.la

#libgtk_boxml_graphic_device_la_LIBADD = \
  #$(PANGOX_LIBS) \
  #$(GTK_LIBS) \
  #$(top_builddir)/src/formatter/areamodel/libareamodel.la \
  #$(top_builddir)/src/formatter/gtk_areamodel/libgtk_areamodel.la \
  #$(top_builddir)/src/formatter/boxml_graphic_device/libboxml_graphic_device.la

#libgtk_boxml_graphic_device_la_LDFLAGS = -version-info @AREAMODEL_VERSION_INFO@

libgtk_boxml_graphic_device_la_SOURCES = Gtk_BoxGraphicDevice.cc

noinst_HEADERS = Gtk_BoxGraphicDevice.hh

INCLUDES = \
  $(PANGOX_CFLAGS) \
  $(GTK_CFLAGS) \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/gtk_areamodel \
  -I$(top_srcdir)/src/formatter/boxml_graphic_device
