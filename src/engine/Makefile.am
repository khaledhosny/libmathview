
SUBDIRS = boxml mathml

lib_LTLIBRARIES = libengine.la

libengine_la_LIBADD = \
  -lstdc++ \
  $(DOM_LIBS) \
  $(top_builddir)/src/engine/mathml/libmathml.la \
  $(top_builddir)/src/engine/boxml/libboxml.la

libengine_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libengine_la_SOURCES = \
  Attribute.cc \
  AttributeList.cc \
  AttributeSignature.cc \
  RefinementContext.cc

noinst_HEADERS = \
  Attribute.hh \
  AttributeList.hh \
  AttributeSignature.hh \
  AbstractRefinementContext.hh \
  RefinementContext.hh \
  BinContainerTemplate.hh \
  LinearContainerTemplate.hh \
  View.hh \
  ViewContext.hh \
  Element.hh 

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/formatter/basictypes
