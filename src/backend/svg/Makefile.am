
NULL =

lib_LTLIBRARIES = libmathview_backend_svg.la

if COND_TFM
MAYBE_TFM_S = \
  SVG_TFMComputerModernShaper.cc \
  SVG_TTF_TFMComputerModernShaper.cc \
  SVG_TTF_TFMComputerModernShaper.hh \
  $(NULL)
MAYBE_TFM_H = \
  SVG_TFMComputerModernShaper.hh \
  SVG_TTF_TFMComputerModernShaper.hh \
  $(NULL)
else
MAYBE_TFM_S = $(NULL)
MAYBE_TFM_H = $(NULL)
endif

libmathview_backend_svg_la_SOURCES = \
  SVG_AreaFactory.hh \
  SVG_Backend.cc \
  SVG_Backend.hh \
  SVG_BackgroundArea.cc \
  SVG_BackgroundArea.hh \
  SVG_ColorArea.cc \
  SVG_ColorArea.hh \
  SVG_InkArea.cc \
  SVG_InkArea.hh \
  SVG_MathGraphicDevice.cc \
  SVG_MathGraphicDevice.hh \
  SVG_RenderingContext.cc \
  SVG_RenderingContext.hh \
  SVG_StreamRenderingContext.cc \
  SVG_StreamRenderingContext.hh \
  SVG_TFMGlyphArea.cc \
  SVG_TFMGlyphArea.hh \
  SVG_TTF_TFMGlyphArea.cc \
  SVG_TTF_TFMGlyphArea.hh \
  $(MAYBE_TFM_S) \
  $(MAYBE_TFM_H) \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/backend/common \
  -I$(top_srcdir)/src/backend/common/tfm \
  $(NULL)
