
NULL =

noinst_LTLIBRARIES = libmathml_graphic_device.la

#libmathml_graphic_device_la_LIBADD =

libmathml_graphic_device_la_SOURCES = \
  MathFormattingContext.cc \
  MathGraphicDevice.cc \
  Shaper.cc \
  ShaperManager.cc \
  ShapingResult.cc \
  SpaceShaper.cc \
  StandardSymbolsShaper.cc \
  $(NULL)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  MathFormattingContext.hh \
  MathGraphicDevice.hh \
  GlyphSpec.hh \
  Shaper.hh \
  ShaperManager.hh \
  ShapingResult.hh \
  SpaceShaper.hh \
  StandardSymbolsShaper.hh \
  $(NULL)

INCLUDES = \
  -I$(top_srcdir)/src \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml

