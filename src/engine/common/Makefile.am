
NULL =

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

pkginclude_HEADERS = \
  View.hh \
  $(NULL)

noinst_HEADERS = \
  Adapters.hh \
  Attribute.hh \
  AttributeList.hh \
  AttributeSignature.hh \
  BinContainerTemplate.hh \
  CommonStringParsers.hh \
  Configuration.hh \
  Element.hh \
  ElementFactory.hh \
  Globals.hh \
  LinearContainerTemplate.hh \
  NamespaceContext.hh \
  Node.hh \
  TemplateStringParsers.hh \
  TemplateStringScanners.hh \
  $(NULL)

INCLUDES = \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/gtk_areamodel

