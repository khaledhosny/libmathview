
NULL =

noinst_LTLIBRARIES = libboxml.la

libboxml_la_SOURCES = \
  BoxMLNamespaceContext.cc \
  BoxMLAttributeSignatures.cc \
  BoxMLElement.cc \
  BoxMLActionElement.cc \
  BoxMLBinContainerElement.cc \
  BoxMLLinearContainerElement.cc \
  BoxMLboxElement.cc \
  BoxMLTextElement.cc \
  BoxMLHElement.cc \
  BoxMLVElement.cc \
  BoxMLGroupElement.cc \
  BoxMLLayoutElement.cc \
  BoxMLAtElement.cc \
  BoxMLSpaceElement.cc \
  BoxMLInkElement.cc

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  BoxMLNamespaceContext.hh \
  BoxMLElement.hh \
  $(NULL)
      
noinst_HEADERS = \
  BoxML.hh \
  BoxMLAttributeSignatures.hh \
  BoxMLAttributeParsers.hh \
  BoxMLActionElement.hh \
  BoxMLBinContainerElement.hh \
  BoxMLLinearContainerElement.hh \
  BoxMLboxElement.hh \
  BoxMLTextElement.hh \
  BoxMLHElement.hh \
  BoxMLVElement.hh \
  BoxMLGroupElement.hh \
  BoxMLLayoutElement.hh \
  BoxMLAtElement.hh \
  BoxMLSpaceElement.hh \
  BoxMLInkElement.hh

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/boxml_graphic_device

# engine/mathml for the operator dictionary in Globals
