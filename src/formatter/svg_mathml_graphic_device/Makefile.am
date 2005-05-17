
NULL =

noinst_LTLIBRARIES = libsvg_mathml_graphic_device.la

if COND_TFM
MAYBE_TFM_S = \
  SVG_TFMComputerModernShaper.cc \
  SVG_TFMComputerModernShaper.hh \
  SVG_TTF_TFMComputerModernShaper.cc \
  SVG_TTF_TFMComputerModernShaper.hh \
  $(NULL)
else
MAYBE_TFM_S = $(NULL)
endif

libsvg_mathml_graphic_device_la_SOURCES = \
  SVG_MathGraphicDevice.cc \
  SVG_MathGraphicDevice.hh \
  $(MAYBE_TFM_S) \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/common/tfm \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/svg_areamodel \
  -I$(top_srcdir)/src/formatter/mathml_graphic_device
