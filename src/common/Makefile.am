
NULL =

SUBDIRS = mathvariants

noinst_LTLIBRARIES = libcommon.la

EXTRA_DIST = \
  Char.hh.in \
  Char.hh

# Char.hh should not be distributed but omitting it makes
# distcheck fail.

libcommon_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libcommon_la_LIBADD = $(top_builddir)/src/common/mathvariants/libmathvariants.la

libcommon_la_SOURCES = \
  BoundingBox.cc \
  BoundingBoxAux.cc \
  Clock.cc \
  LengthAux.cc \
  Logger.cc \
  PointAux.cc \
  RGBColorAux.cc \
  Rectangle.cc \
  String.cc \
  ValueConversion.cc \
  token.cc \
  $(NULL)

pkginclude_HEADERS = defs.h

noinst_HEADERS = \
  BoundingBox.hh \
  BoundingBoxAux.hh \
  Clock.hh \
  GObjectPtr.hh \
  HashMap.hh \
  Length.hh \
  LengthAux.hh \
  Logger.hh \
  Object.hh \
  Point.hh \
  PointAux.hh \
  RGBColor.hh \
  RGBColorAux.hh \
  Rectangle.hh \
  Rectangle.hh \
  Rope.hh \
  ScopedHashMap.hh \
  SmartPtr.hh \
  SparseMap.hh \
  String.hh \
  TokenSet.hh \
  Value.hh \
  ValueConversion.hh \
  Variant.hh \
  WeakPtr.hh \
  fixed.hh \
  for_each_if.h \
  scaled.hh \
  scaledAux.hh \
  token.hh \
  $(NULL)

INCLUDES = \
  $(GLIB_CFLAGS) \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common


