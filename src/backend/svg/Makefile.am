
NULL =

lib_LTLIBRARIES = libmathview_backend_svg.la

if COND_BOXML
MAYBE_BOXML_S = SVG_BoxGraphicDevice.cc
MAYBE_BOXML_H = SVG_BoxGraphicDevice.hh
else
MAYBE_BOXML_S = $(NULL)
MAYBE_BOXML_H = $(NULL)
endif

if COND_TFM
MAYBE_TFM_S = \
  SVG_TFMComputerModernMathGraphicDevice.cc \
  SVG_TFMComputerModernShaper.cc \
  SVG_TTF_TFMComputerModernShaper.cc \
  SVG_TTF_TFMComputerModernShaper.hh \
  $(NULL)
MAYBE_TFM_H = \
  SVG_TFMComputerModernMathGraphicDevice.hh \
  SVG_TFMComputerModernShaper.hh \
  SVG_TTF_TFMComputerModernShaper.hh \
  $(NULL)
else
MAYBE_TFM_S = $(NULL)
MAYBE_TFM_H = $(NULL)
endif

libmathview_backend_svg_la_CPPFLAGS = -DGMV_BackEnd_DLL
libmathview_backend_svg_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_backend_svg_la_SOURCES = \
  SVG_AreaFactory.hh \
  SVG_Backend.cc \
  SVG_BackgroundArea.cc \
  SVG_BackgroundArea.hh \
  SVG_ColorArea.cc \
  SVG_ColorArea.hh \
  SVG_InkArea.cc \
  SVG_InkArea.hh \
  SVG_MathGraphicDevice.cc \
  SVG_MathGraphicDevice.hh \
  SVG_RenderingContext.cc \
  SVG_StreamRenderingContext.cc \
  SVG_TFMGlyphArea.cc \
  SVG_TFMGlyphArea.hh \
  SVG_TTF_TFMGlyphArea.cc \
  SVG_TTF_TFMGlyphArea.hh \
  SVG_WrapperArea.cc \
  SVG_WrapperArea.hh \
  $(MAYBE_BOXML_S) \
  $(MAYBE_BOXML_H) \
  $(MAYBE_TFM_S) \
  $(MAYBE_TFM_H) \
  $(NULL)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  SVG_Backend.hh \
  SVG_RenderingContext.hh \
  SVG_StreamRenderingContext.hh \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/backend/common \
  -I$(top_srcdir)/src/backend/common/tfm \
  $(NULL)
