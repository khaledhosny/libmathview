
NULL =

bin_PROGRAMS = mathmlsvg

mathmlsvg_SOURCES = \
  main_gmetadom.cc \
  $(NULL)

mathmlsvg_LDADD = \
  $(top_builddir)/src/view/libmathview_gmetadom.la \
  $(NULL)

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/gmetadom \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/svg_areamodel \
  -I$(top_srcdir)/src/formatter/mathml_graphic_device \
  -I$(top_srcdir)/src/formatter/svg_mathml_graphic_device \
  -I$(top_srcdir)/src/view \
  $(NULL)
