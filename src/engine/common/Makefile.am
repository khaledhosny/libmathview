
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
  NamespaceRegistry.cc \
  NamespaceContext.cc \
  ElementFactory.cc \
  View.cc \
  DOMView.cc \
  Node.cc \
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
  NamespaceRegistry.hh \
  NamespaceContext.hh \
  ElementFactory.hh \
  View.hh \
  DOMView.hh \
  Node.hh \
  Element.hh \
  Linker.hh

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/gtk_areamodel
