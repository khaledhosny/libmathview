
NULL =

noinst_LTLIBRARIES = libareamodel.la

#libareamodel_la_LIBADD = -lstdc++

libareamodel_la_LDFLAGS = -version-info @AREAMODEL_VERSION_INFO@

if COND_TFM
MAYBE_TFM_S = \
  TFMGlyphArea.cc \
  $(NULL)
MAYBE_TFM_H = \
  TFMGlyphArea.hh \
  $(NULL)
else
MAYBE_TFM_S = $(NULL)
MAYBE_TFM_H = $(NULL)
endif

libareamodel_la_SOURCES = \
  Area.cc \
  AreaFactory.cc \
  AreaId.cc \
  AreaIdAux.cc \
  BinContainerArea.cc \
  BoxArea.cc \
  BoxedLayoutArea.cc \
  ColorArea.cc \
  GlyphArea.cc \
  GlyphStringArea.cc \
  GlyphWrapperArea.cc \
  HideArea.cc \
  HorizontalArrayArea.cc \
  HorizontalFillerArea.cc \
  HorizontalSpaceArea.cc \
  IdArea.cc \
  IgnoreArea.cc	\
  InkArea.cc \
  LinearContainerArea.cc \
  OverlapArrayArea.cc \
  ShiftArea.cc \
  SimpleArea.cc \
  VerticalArrayArea.cc \
  VerticalFillerArea.cc \
  VerticalSpaceArea.cc \
  WrapperArea.cc \
  $(MAYBE_TFM_S) \
  $(NULL)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  Area.hh \
  AreaFactory.hh \
  AreaId.hh \
  AreaIdAux.hh \
  BinContainerArea.hh \
  BoxArea.hh \
  BoxedLayoutArea.hh \
  ColorArea.hh	\
  ContainerArea.hh \
  FillerArea.hh \
  GlyphArea.hh \
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
  OverlapArrayArea.hh \
  RenderingContext.hh \
  SearchingContext.hh \
  ShiftArea.hh \
  SimpleArea.hh \
  VerticalArrayArea.hh \
  VerticalFillerArea.hh \
  VerticalSpaceArea.hh \
  WrapperArea.hh \
  $(MAYBE_TFM_H) \
  $(NULL)

noinst_HEADERS = \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/engine/common

