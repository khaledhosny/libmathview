
lib_LTLIBRARIES = libboxml.la

libboxml_la_SOURCES = \
  BoxMLAttributeSignatures.cc \
  BoxMLElement.cc \
  BoxMLBinContainerElement.cc \
  BoxMLLinearContainerElement.cc \
  BoxMLboxElement.cc \
  BoxMLTextElement.cc \
  BoxMLHElement.cc \
  BoxMLVElement.cc \
  BoxMLGElement.cc \
  BoxMLAtElement.cc \
  BoxMLSpaceElement.cc \
  BoxMLInkElement.cc

noinst_HEADERS = \
  BoxMLAttributeSignatures.hh \
  BoxMLAttributeParsers.hh \
  BoxMLElement.hh \
  BoxMLBinContainerElement.hh \
  BoxMLLinearContainerElement.hh \
  BoxMLboxElement.hh \
  BoxMLTextElement.hh \
  BoxMLHElement.hh \
  BoxMLVElement.hh \
  BoxMLGElement.hh \
  BoxMLAtElement.hh \
  BoxMLSpaceElement.hh \
  BoxMLInkElement.hh

INCLUDES = \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/formatter/basictypes/mathvariants/ \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/gtk_areamodel \
  -I$(top_srcdir)/src/formatter/mathml_graphic_device \
  -I$(top_srcdir)/src/formatter/gtk_mathml_graphic_device \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml
