
noinst_LTLIBRARIES = libmathml_graphic_device.la

libmathml_graphic_device_la_LIBADD =

libmathml_graphic_device_la_SOURCES = \
  MathFormattingContext.cc \
  MathGraphicDevice.cc \
  ShaperManager.cc

noinst_HEADERS = \
  MathFormattingContext.hh \
  MathGraphicDevice.hh \
  GlyphSpec.hh \
  Shaper.hh \
  ShaperManager.hh

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/formatter/basictypes \
  -I$(top_srcdir)/src/formatter/basictypes/mathvariants \
  -I$(top_srcdir)/src/formatter/areamodel

