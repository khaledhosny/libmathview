
noinst_LTLIBRARIES = libcommon.la

EXTRA_DIST = \
  Char.hh.in \
  Char.hh

# Char.hh should not be distributed but omitting it makes
# distcheck fail.

libcommon_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libcommon_la_SOURCES = \
  Clock.cc \
  Logger.cc \
  Rectangle.cc \
  scaledConv.cc \
  token.cc \
  RGBColorAux.cc \
  BoundingBox.cc \
  BoundingBoxAux.cc \
  LengthAux.cc \
  String.cc \
  ValueConversion.cc

pkginclude_HEADERS = defs.h

noinst_HEADERS = \
  Clock.hh \
  Logger.hh \
  Rectangle.hh \
  token.hh \
  TokenSet.hh \
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
  Rope.hh \
  ScopedHashMap.hh \
  SparseMap.hh \
  SmartPtr.hh \
  WeakPtr.hh \
  String.hh \
  GObjectPtr.hh \
  Object.hh

INCLUDES = \
  $(GLIB_CFLAGS) \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common


