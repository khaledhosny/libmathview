
lib_LTLIBRARIES = libgtk_mathml_graphic_device.la

libgtk_mathml_graphic_device_la_LIBADD = \
  $(PANGOX_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/formatter/mathml_graphic_device/libgtk_areamodel.la

libgtk_mathml_graphic_device_la_LDFLAGS = -version-info @AREAMODEL_VERSION_INFO@

libgtk_mathml_graphic_device_la_SOURCES = \
  Gtk_MathGraphicDevice.cc

INCLUDES = \
  $(DOM_CFLAGS)
  
