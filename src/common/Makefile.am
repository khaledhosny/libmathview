
lib_LTLIBRARIES = libcommon.la

libcommon_la_LIBADD = \
  -lstdc++

libcommon_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libcommon_la_SOURCES = \
  Clock.cc \
  Logger.cc \
  ChildList.cc \
  Rectangle.cc \
  gmetadom.cc \
  scaledConv.cc \
  token.cc \
  RGBColorAux.cc \
  BoundingBox.cc \
  BoundingBoxAux.cc \
  LengthAux.cc \
  String.cc \
  ValueConversion.cc

noinst_HEADERS = \
  Clock.hh \
  Logger.hh \
  ChildList.hh \
  Rectangle.hh \
  gmetadom.hh \
  token.hh \
  scaledConv.hh \
  for_each_if.h \
  fixed.hh \
  scaled.hh \
  scaledAux.hh \
  Length.hh \
  LengthAux.hh \
  RGBColor.hh \
  RGBColorAux.hh \
  BoundingBox.hh \
  BoundingBoxAux.hh \
  Rectangle.hh \
  Value.hh \
  ValueConversion.hh \
  Variant.hh \
  HashMap.hh \
  ScopedHashMap.hh \
  SparseMap.hh \
  SmartPtr.hh \
  String.hh \
  GObjectPtr.hh \
  Object.hh \
  DOM.hh

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/auto

