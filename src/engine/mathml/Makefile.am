
lib_LTLIBRARIES = libmathml.la

libmathml_la_SOURCES = \
  MathMLAttributeSignatures.cc

noinst_HEADERS = \
  MathMLAttributeParsers.hh \
  MathMLAttributeSignatures.hh

INCLUDES = \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src \
  -I$(top_srcdir)/src/formatter/basictypes \
  -I$(top_srcdir)/src/formatter/basictypes/mathvariants/ \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/gtk_areamodel \
  -I$(top_srcdir)/src/formatter/mathml_graphic_device \
  -I$(top_srcdir)/src/formatter/gtk_mathml_graphic_device \
  -I$(top_srcdir)/src/engine

