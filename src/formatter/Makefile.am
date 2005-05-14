
NULL =

if COND_BOXML
MAYBE_BOXML_SUBDIRS = \
  boxml_graphic_device \
  gtk_boxml_graphic_device \
  $(NULL)
else
MAYBE_BOXML_SUBDIRS = $(NULL)
endif

if COND_SVG
MAYBE_SVG_SUBDIRS = \
  svg_areamodel \
  svg_mathml_graphic_device \
  $(NULL)
else
MAYBE_SVG_SUBDIRS = $(NULL)
endif

SUBDIRS = \
  common \
  areamodel \
  gtk_areamodel \
  mathml_graphic_device \
  gtk_mathml_graphic_device \
  $(MAYBE_BOXML_SUBDIRS) \
  $(MAYBE_SVG_SUBDIRS) \
  $(NULL)

noinst_LTLIBRARIES = libformatter.la

if COND_BOXML
MAYBE_BOXML_LIBADD = \
  $(top_builddir)/src/formatter/gtk_boxml_graphic_device/libgtk_boxml_graphic_device.la \
  $(top_builddir)/src/formatter/boxml_graphic_device/libboxml_graphic_device.la \
  $(NULL)
else
MAYBE_BOXML_LIBADD = $(NULL)
endif

if COND_SVG
MAYBE_SVG_LIBADD = \
  $(top_builddir)/src/formatter/svg_mathml_graphic_device/libsvg_mathml_graphic_device.la \
  $(top_builddir)/src/formatter/svg_areamodel/libsvg_areamodel.la \
  $(NULL)
else
MAYBE_SVG_LIBADD = $(NULL)
endif

libformatter_la_LIBADD = \
  $(top_builddir)/src/formatter/gtk_mathml_graphic_device/libgtk_mathml_graphic_device.la \
  $(top_builddir)/src/formatter/mathml_graphic_device/libmathml_graphic_device.la \
  $(top_builddir)/src/formatter/gtk_areamodel/libgtk_areamodel.la \
  $(top_builddir)/src/formatter/areamodel/libareamodel.la \
  $(top_builddir)/src/formatter/common/libcommon.la \
  $(MAYBE_BOXML_LIBADD) \
  $(MAYBE_SVG_LIBADD) \
  $(NULL)

libformatter_la_SOURCES =

