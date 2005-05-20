
NULL =

if COND_TFM
MAYBE_TFM_SUBDIRS = tfm
MAYBE_LIBTFM = $(top_builddir)/src/backend/common/tfm/libtfm.la
else
MAYBE_TFM_SUBDIRS =
MAYBE_LIBTFM =
endif

SUBDIRS = $(MAYBE_TFM_SUBDIRS)

noinst_LTLIBRARIES = libbackend_common.la

libbackend_common_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

if COND_BOXML
MAYBE_BOXML_S = \
  BoxGraphicDevice.cc \
  $(NULL)
MAYBE_BOXML_H = \
  BoxGraphicDevice.hh \
  $(NULL)
else
MAYBE_BOXML_S = $(NULL)
MAYBE_BOXML_H = $(NULL)
endif

if COND_TFM
MAYBE_TFM_S = \
  TFMComputerModernShaper.cc \
  TFMFont.cc \
  TFMFontManager.cc \
  TFMGlyphArea.cc \
  $(NULL)
MAYBE_TFM_H = \
  TFMComputerModernShaper.hh \
  TFMFont.hh \
  TFMFontManager.hh \
  TFMGlyphArea.hh \
  $(NULL)
else
MAYBE_TFM_S = $(NULL)
MAYBE_TFM_H = $(NULL)
endif

if COND_T1LIB
if COND_TFM
MAYBE_T1LIB_TFM_S = \
  t1lib_TFM_T1Font.cc \
  t1lib_TFM_T1FontManager.cc \
  $(NULL)
MAYBE_T1LIB_TFM_H = \
  t1lib_TFM_T1Font.hh \
  t1lib_TFM_T1FontManager.hh \
  $(NULL)
else
MAYBE_T1LIB_TFM_S = $(NULL)
MAYBE_T1LIB_TFM_H = $(NULL)
endif
MAYBE_T1LIB_S = \
  t1lib_T1Font.cc \
  t1lib_T1FontManager.cc \
  $(MAYBE_T1LIB_TFM_S) \
  $(NULL)
MAYBE_T1LIB_H = \
  t1lib_T1Font.hh \
  t1lib_T1FontManager.hh \
  $(MAYBE_T1LIB_TFM_H) \
  $(NULL)
else
MAYBE_T1LIB_S = $(NULL)
MAYBE_T1LIB_H = $(NULL)
endif

libbackend_common_la_SOURCES = \
  Area.cc \
  AreaFactory.cc \
  AreaId.cc \
  AreaIdAux.cc \
  Backend.cc \
  BinContainerArea.cc \
  BoxArea.cc \
  BoxedLayoutArea.cc \
  ColorArea.cc \
  ComputerModernShaper.cc \
  FormattingContext.cc \
  FormattingContext.hh \
  GlyphArea.cc \
  GlyphStringArea.cc \
  GlyphWrapperArea.cc \
  GraphicDevice.cc \
  GraphicDevice.hh \
  HideArea.cc \
  HorizontalArrayArea.cc \
  HorizontalFillerArea.cc \
  HorizontalSpaceArea.cc \
  IdArea.cc \
  IgnoreArea.cc	\
  InkArea.cc \
  LinearContainerArea.cc \
  MathGraphicDevice.cc \
  MathGraphicDevice.hh \
  NullShaper.cc \
  OverlapArrayArea.cc \
  Shaper.cc \
  ShaperManager.cc \
  ShapingContext.cc \
  ShiftArea.cc \
  SimpleArea.cc \
  SpaceShaper.cc \
  StandardSymbolsShaper.cc \
  VerticalArrayArea.cc \
  VerticalFillerArea.cc \
  VerticalSpaceArea.cc \
  WrapperArea.cc \
  $(MAYBE_BOXML_S) \
  $(MAYBE_TFM_S) \
  $(MAYBE_T1LIB_S) \
  $(NULL)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  Area.hh \
  AreaFactory.hh \
  AreaId.hh \
  AreaIdAux.hh \
  Backend.hh \
  BinContainerArea.hh \
  BoxArea.hh \
  BoxedLayoutArea.hh \
  CachedShapedString.hh \
  ColorArea.hh	\
  ComputerModernShaper.hh \
  ContainerArea.hh \
  FillerArea.hh \
  GlyphArea.hh \
  GlyphSpec.hh \
  GlyphStringArea.hh \
  GlyphWrapperArea.hh \
  HideArea.hh \
  HorizontalArrayArea.hh \
  HorizontalFillerArea.hh \
  HorizontalSpaceArea.hh \
  IdArea.hh \
  IgnoreArea.hh	\
  InkArea.hh \
  LinearContainerArea.hh \
  NullShaper.hh \
  OverlapArrayArea.hh \
  RenderingContext.hh \
  SearchingContext.hh \
  Shaper.hh \
  ShaperManager.hh \
  ShapingContext.hh \
  ShiftArea.hh \
  SimpleArea.hh \
  SpaceShaper.hh \
  StandardSymbolsShaper.hh \
  T1Font.hh \
  VerticalArrayArea.hh \
  VerticalFillerArea.hh \
  VerticalSpaceArea.hh \
  WrapperArea.hh \
  $(MAYBE_TFM_H) \
  $(MAYBE_T1LIB_H) \
  $(MAYBE_BOXML_H) \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/backend/common/tfm \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  $(NULL)

libbackend_common_la_LIBADD = \
  $(MAYBE_LIBTFM) \
  $(NULL)

