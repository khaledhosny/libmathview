
NULL =

noinst_LTLIBRARIES = libmathml_graphic_device.la

#libmathml_graphic_device_la_LIBADD =

libmathml_graphic_device_la_SOURCES = \
  MathGraphicDevice.cc \
  Shaper.cc \
  ShaperManager.cc \
  ShapingContext.cc \
  SpaceShaper.cc \
  NullShaper.cc \
  StandardSymbolsShaper.cc \
  ComputerModernShaper.cc \
  $(NULL)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  MathGraphicDevice.hh \
  GlyphSpec.hh \
  Shaper.hh \
  ShaperManager.hh \
  ShapingContext.hh \
  SpaceShaper.hh \
  NullShaper.hh \
  StandardSymbolsShaper.hh \
  ComputerModernShaper.hh \
  CachedShapedString.hh \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml

