
NULL =

SUBDIRS = 

noinst_LTLIBRARIES = libbackend_common.la

libbackend_common_la_CPPFLAGS = -DGMV_MathView_DLL
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
  CombinedGlyphArea.cc \
  FormattingContext.cc \
  GlyphArea.cc \
  GlyphStringArea.cc \
  GlyphWrapperArea.cc \
  GraphicDevice.cc \
  HideArea.cc \
  HorizontalArrayArea.cc \
  HorizontalFillerArea.cc \
  HorizontalSpaceArea.cc \
  IdArea.cc \
  IgnoreArea.cc	\
  InkArea.cc \
  LinearContainerArea.cc \
  MathGraphicDevice.cc \
  MathFont.cc \
  NullShaper.cc \
  OverlapArrayArea.cc \
  OpenTypeShaper.cc \
  OpenTypeMathGraphicDevice.cc \
  Shaper.cc \
  ShaperManager.cc \
  ShapingContext.cc \
  ShiftArea.cc \
  SimpleArea.cc \
  SpaceShaper.cc \
  StepArea.cc \
  VerticalArrayArea.cc \
  VerticalFillerArea.cc \
  VerticalSpaceArea.cc \
  WrapperArea.cc \
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
  CombinedGlyphArea.hh \
  ContainerArea.hh \
  FillerArea.hh \
  FormattingContext.hh \
  GlyphArea.hh \
  GlyphSpec.hh \
  GlyphStringArea.hh \
  GlyphWrapperArea.hh \
  GraphicDevice.hh \
  HideArea.hh \
  HorizontalArrayArea.hh \
  HorizontalFillerArea.hh \
  HorizontalSpaceArea.hh \
  IdArea.hh \
  IgnoreArea.hh	\
  InkArea.hh \
  LinearContainerArea.hh \
  MathFont.hh \
  MathGraphicDevice.hh \
  MathTable.hh \
  NullShaper.hh \
  OverlapArrayArea.hh \
  OpenTypeMathGraphicDevice.hh \
  OpenTypeShaper.hh \
  RenderingContext.hh \
  SearchingContext.hh \
  Shaper.hh \
  ShaperManager.hh \
  ShapingContext.hh \
  ShiftArea.hh \
  SimpleArea.hh \
  SpaceShaper.hh \
  StepArea.hh \
  VerticalArrayArea.hh \
  VerticalFillerArea.hh \
  VerticalSpaceArea.hh \
  WrapperArea.hh \
  $(NULL)

libbackend_common_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  $(NULL)

libbackend_common_la_LIBADD = \
  $(NULL)


-include $(top_srcdir)/git.mk
