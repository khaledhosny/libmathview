
SUBDIRS = mathvariants

noinst_LTLIBRARIES = libbasictypes.la

noinst_PROGRAMS = test1

test1_SOURCES = test-fixed.cc

test1_LDADD = -lstdc++

libbasictypes_la_SOURCES = \
  RGBColorAux.cc \
  BoundingBox.cc \
  BoundingBoxAux.cc \
  LengthAux.cc

libbasictypes_la_LIBADD = \
  -lstdc++ \
  $(top_builddir)/src/formatter/basictypes/mathvariants/libmathvariants.la

noinst_HEADERS = \
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
  Variant.hh \
  ScopedHashMap.hh \
  SparseMap.hh \
  SmartPtr.hh \
  GObjectPtr.hh \
  Object.hh \
  DOM.hh

