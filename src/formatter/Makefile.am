
SUBDIRS = \
  common \
  basictypes \
  areamodel \
  gtk_areamodel \
  mathml_graphic_device \
  gtk_mathml_graphic_device \
  boxml_graphic_device \
  gtk_boxml_graphic_device

noinst_LTLIBRARIES = libformatter.la

libformatter_la_LIBADD = \
  $(top_builddir)/src/formatter/gtk_mathml_graphic_device/libgtk_mathml_graphic_device.la \
  $(top_builddir)/src/formatter/mathml_graphic_device/libmathml_graphic_device.la \
  $(top_builddir)/src/formatter/gtk_boxml_graphic_device/libgtk_boxml_graphic_device.la \
  $(top_builddir)/src/formatter/boxml_graphic_device/libboxml_graphic_device.la \
  $(top_builddir)/src/formatter/gtk_areamodel/libgtk_areamodel.la \
  $(top_builddir)/src/formatter/areamodel/libareamodel.la \
  $(top_builddir)/src/formatter/basictypes/mathvariants/libmathvariants.la

libformatter_la_SOURCES =
