
NULL =

SUBDIRS = mathvariants

noinst_LTLIBRARIES = libcommon.la

libcommon_la_CPPFLAGS = $(NULL)
libcommon_la_CPPFLAGS += -DGMV_MathView_DLL
libcommon_la_LIBADD = \
  $(top_builddir)/src/common/mathvariants/libmathvariants.la \
  $(GLIB_LIBS) \
  $(NULL)

# if USE_GCC_PCH
# BUILT_SOURCES = all.hh
# include ./$(DEPDIR)/all.Po
# include $(top_srcdir)/gcc-pch.mk
# libcommon_la_CPPFLAGS += -include all.hh
# endif

libcommon_la_SOURCES = \
  AbstractLogger.cc \
  BoundingBox.cc \
  BoundingBoxAux.cc \
  Clock.cc \
  Configuration.cc \
  LengthAux.cc \
  Logger.cc \
  PointAux.cc \
  RGBColorAux.cc \
  Rectangle.cc \
  String.cc \
  StringAux.cc \
  StringHash.cc \
  Utils.cc \
  ValueConversion.cc \
  token.cc \
  $(NULL)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  AbstractLogger.hh \
  BoundingBox.hh \
  BoundingBoxAux.hh \
  Configuration.hh \
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
  SmartPtr.hh \
  ScopedHashMap.hh \
  FastScopedHashMap.hh \
  SparseMap.hh \
  String.hh \
  StringAux.hh \
  StringHash.hh \
  Utils.hh \
  Value.hh \
  ValueConversion.hh \
  Variant.hh \
  WeakPtr.hh \
  defs.h \
  fixed.hh \
  gmv_defines.h \
  scaled.hh \
  scaledAux.hh \
  $(NULL)

noinst_HEADERS = \
  CharTraits.hh \
  CharTraits.icc \
  Clock.hh \
  GObjectPtr.hh \
  TemplateStringScanners.hh \
  TemplateStringParsers.hh \
  TokenSet.hh \
  for_each_if.h \
  token.hh \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  $(GLIB_CFLAGS) \
  $(NULL)

