
noinst_LTLIBRARIES = libareamodel.la

libareamodel_la_LIBADD = \
  -lstdc++ \
  $(top_builddir)/src/formatter/basictypes/libbasictypes.la

libareamodel_la_LDFLAGS = -version-info @AREAMODEL_VERSION_INFO@

libareamodel_la_SOURCES = \
  Area.cc \
  AreaFactory.cc \
  BoxArea.cc \
  BoxedLayoutArea.cc \
  ColorArea.cc \
  HideArea.cc \
  HorizontalFillerArea.cc \
  IdArea.cc \
  InkArea.cc \
  OverlapArrayArea.cc \
  VerticalArrayArea.cc \
  AreaId.cc \
  HorizontalSpaceArea.cc \
  ReplacementContext.cc \
  VerticalFillerArea.cc \
  AreaIdFactory.cc \
  HideArea.cc \
  IgnoreArea.cc	\
  ShiftArea.cc \
  VerticalSpaceArea.cc \
  BinContainerArea.cc \
  HorizontalArrayArea.cc \
  LinearContainerArea.cc \
  SimpleArea.cc

pkginclude_HEADERS = \
  Area.hh \
  AreaFactory.hh \
  BoxedLayoutArea.hh \
  ColorArea.hh	\
  HorizontalFillerArea.hh \
  ShiftArea.hh \
  VerticalSpaceArea.hh \
  AreaId.hh \
  ContainerArea.hh \
  HorizontalSpaceArea.hh \
  OverlapArrayArea.hh \
  SimpleArea.hh \
  AreaIdFactory.hh \
  FillerArea.hh \
  IdArea.hh \
  InkArea.hh \
  BinContainerArea.hh \
  IgnoreArea.hh	\
  RenderingContext.hh \
  HideArea.hh \
  LinearContainerArea.hh \
  ReplacementContext.hh \
  VerticalArrayArea.hh \
  BoxArea.hh \
  HorizontalArrayArea.hh \
  VerticalFillerArea.hh

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/formatter/basictypes

