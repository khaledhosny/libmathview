
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

pkginclude_HEADERS = \
  BoundingBox.hh \
  BoundingBoxAux.hh \
  Char.hh \
  Object.hh \
  Point.hh \
  PointAux.hh \
  RGBColor.hh \
  RGBColorAux.hh \
  Rectangle.hh \
  Rectangle.hh \
  SmartPtr.hh \
  String.hh \
  WeakPtr.hh \
  defs.h \
  fixed.hh \
  scaled.hh \
  scaledAux.hh \
  $(NULL)

noinst_HEADERS = \
  Clock.hh \
  GObjectPtr.hh \
  HashMap.hh \
  Length.hh \
  LengthAux.hh \
  Logger.hh \
  Rope.hh \
  ScopedHashMap.hh \
  SparseMap.hh \
  TokenSet.hh \
  Value.hh \
  ValueConversion.hh \
  Variant.hh \
  for_each_if.h \
  token.hh \
  $(NULL)

INCLUDES = \
  $(GLIB_CFLAGS) \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common


