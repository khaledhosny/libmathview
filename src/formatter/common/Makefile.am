
NULL =

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  FormattingContext.hh \
  GraphicDevice.hh \
  $(NULL)

noinst_LTLIBRARIES = libcommon.la

libcommon_la_SOURCES = \
  FormattingContext.cc \
  GraphicDevice.cc \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/mathml_graphic_device \
  -I$(top_srcdir)/src/formatter/boxml_graphic_device \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  $(NULL)

