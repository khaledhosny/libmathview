
lib_LTLIBRARIES = libboxml.la

libboxml_la_SOURCES = \
  BoxedParagraph.cc \
  BoxMLElementFactory.cc \
  BoxMLNamespaceContext.cc \
  BoxMLAttributeSignatures.cc \
  BoxMLElement.cc \
  BoxMLActionElement.cc \
  BoxMLBinContainerElement.cc \
  BoxMLLinearContainerElement.cc \
  BoxMLboxElement.cc \
  BoxMLObjectElement.cc \
  BoxMLTextElement.cc \
  BoxMLHElement.cc \
  BoxMLVElement.cc \
  BoxMLGroupElement.cc \
  BoxMLParagraphElement.cc \
  BoxMLLayoutElement.cc \
  BoxMLAtElement.cc \
  BoxMLSpaceElement.cc \
  BoxMLInkElement.cc

noinst_HEADERS = \
  BoxedParagraph.hh \
  BoxMLElementFactory.hh \
  BoxMLNamespaceContext.hh \
  BoxML.hh \
  BoxMLAttributeSignatures.hh \
  BoxMLAttributeParsers.hh \
  BoxMLElement.hh \
  BoxMLActionElement.hh \
  BoxMLBinContainerElement.hh \
  BoxMLLinearContainerElement.hh \
  BoxMLboxElement.hh \
  BoxMLObjectElement.hh \
  BoxMLParagraphElement.hh \
  BoxMLTextElement.hh \
  BoxMLHElement.hh \
  BoxMLVElement.hh \
  BoxMLGroupElement.hh \
  BoxMLLayoutElement.hh \
  BoxMLAtElement.hh \
  BoxMLSpaceElement.hh \
  BoxMLInkElement.hh

INCLUDES = \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/gtk_areamodel \
  -I$(top_srcdir)/src/formatter/boxml_graphic_device \
  -I$(top_srcdir)/src/formatter/gtk_boxml_graphic_device \
  -I$(top_srcdir)/src/formatter/mathml_graphic_device \
  -I$(top_srcdir)/src/formatter/basictypes/mathvariants


# engine/mathml is for Globals.hh
