
if COND_BOXML
MAYBE_BOXML_SUBDIRS = \
  boxml_graphic_device \
  gtk_boxml_graphic_device
endif

SUBDIRS = \
  common \
  areamodel \
  gtk_areamodel \
  mathml_graphic_device \
  gtk_mathml_graphic_device \
  $(MAYBE_BOXML_SUBDIRS)

noinst_LTLIBRARIES = libformatter.la

if COND_BOXML
MAYBE_BOXML_LIBADD = \
  $(top_builddir)/src/formatter/gtk_boxml_graphic_device/libgtk_boxml_graphic_device.la \
  $(top_builddir)/src/formatter/boxml_graphic_device/libboxml_graphic_device.la
endif

libformatter_la_LIBADD = \
  $(top_builddir)/src/formatter/gtk_mathml_graphic_device/libgtk_mathml_graphic_device.la \
  $(top_builddir)/src/formatter/mathml_graphic_device/libmathml_graphic_device.la \
  $(top_builddir)/src/formatter/gtk_areamodel/libgtk_areamodel.la \
  $(top_builddir)/src/formatter/areamodel/libareamodel.la \
  $(MAYBE_BOXML_LIBADD)

libformatter_la_SOURCES =

