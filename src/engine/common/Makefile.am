
lib_LTLIBRARIES = libengine_common.la

libengine_common_la_LIBADD = \
  -lstdc++ \
  $(DOM_LIBS)

libengine_common_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libengine_common_la_SOURCES = \
  Attribute.cc \
  AttributeList.cc \
  AttributeSignature.cc \
  RefinementContext.cc \
  NamespaceFactory.cc \
  ElementFactory.cc \
  Element.cc \
  Linker.cc

noinst_HEADERS = \
  Attribute.hh \
  AttributeList.hh \
  AttributeSignature.hh \
  AbstractRefinementContext.hh \
  RefinementContext.hh \
  BinContainerTemplate.hh \
  LinearContainerTemplate.hh \
  NamespaceFactory.hh \
  ElementFactory.hh \
  View.hh \
  ViewContext.hh \
  Element.hh \
  Linker.hh

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/formatter/areamodel
