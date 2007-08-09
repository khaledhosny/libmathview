
NULL =

lib_LTLIBRARIES = libmathview_backend_ps.la

if COND_BOXML
MAYBE_BOXML_S = PS_BoxGraphicDevice.cc
MAYBE_BOXML_H = PS_BoxGraphicDevice.hh
else
MAYBE_BOXML_S = $(NULL)
MAYBE_BOXML_H = $(NULL)
endif

if COND_TFM
MAYBE_TFM_S = \
  PS_TFMComputerModernMathGraphicDevice.cc \
  PS_TFMComputerModernShaper.cc \
  $(NULL)
MAYBE_TFM_H = \
  PS_TFMComputerModernMathGraphicDevice.hh \
  PS_TFMComputerModernShaper.hh \
  $(NULL)
else
MAYBE_TFM_S = $(NULL)
MAYBE_TFM_H = $(NULL)
endif

if COND_T1LIB
MAYBE_T1LIB_S = T1_FontDataBase.cc
MAYBE_T1LIB_H = T1_FontDataBase.hh
else
MAYBE_T1LIB_S = $(NULL)
MAYBE_T1LIB_H = $(NULL)
endif

libmathview_backend_ps_la_CPPFLAGS = -DGMV_BackEnd_DLL
libmathview_backend_ps_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_backend_ps_la_SOURCES = \
  PS_AreaFactory.hh \
  PS_Backend.cc \
  PS_BackgroundArea.cc \
  PS_BackgroundArea.hh \
  PS_ColorArea.cc \
  PS_ColorArea.hh \
  PS_InkArea.cc \
  PS_InkArea.hh \
  PS_MathGraphicDevice.cc \
  PS_MathGraphicDevice.hh \
  PS_RenderingContext.cc \
  PS_StreamRenderingContext.cc \
  FontDataBase.cc \
  FontDataBase.hh \
  PS_TFMGlyphArea.cc \
  PS_TFMGlyphArea.hh \
  $(MAYBE_BOXML_S) \
  $(MAYBE_BOXML_H) \
  $(MAYBE_TFM_S) \
  $(MAYBE_TFM_H) \
  $(MAYBE_T1LIB_S) \
  $(MAYBE_T1LIB_H) \
  $(NULL)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  PS_Backend.hh \
  PS_RenderingContext.hh \
  PS_StreamRenderingContext.hh \
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
  $(T1_CFLAGS) \
  $(NULL)
