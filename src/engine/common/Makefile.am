
noinst_LTLIBRARIES = libengine_common.la

libengine_common_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libengine_common_la_SOURCES = \
  Globals.cc \
  Attribute.cc \
  AttributeList.cc \
  AttributeSignature.cc \
  NamespaceContext.cc \
  View.cc \
  Node.cc \
  Element.cc

noinst_HEADERS = \
  Globals.hh \
  Configuration.hh \
  Adapters.hh \
  Attribute.hh \
  AttributeList.hh \
  AttributeSignature.hh \
  CommonStringParsers.hh \
  TemplateStringParsers.hh \
  TemplateStringScanners.hh \
  BinContainerTemplate.hh \
  LinearContainerTemplate.hh \
  NamespaceContext.hh \
  ElementFactory.hh \
  View.hh \
  Node.hh \
  Element.hh

INCLUDES = \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/gtk_areamodel

