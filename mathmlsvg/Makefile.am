
NULL =

bin_PROGRAMS = $(NULL)
if COND_LIBXML2_READER
bin_PROGRAMS += mathmlsvg
endif

mathmlsvg_SOURCES = \
  main_libxml2_reader.cc \
  $(NULL)

mathmlsvg_LDADD = \
  $(GLIB_LIBS) \
  $(top_builddir)/src/view/libmathview_libxml2_reader.la \
  $(NULL)

INCLUDES = \
  $(XML_CFLAGS) \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/libxml2_reader \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/svg_areamodel \
  -I$(top_srcdir)/src/formatter/mathml_graphic_device \
  -I$(top_srcdir)/src/formatter/svg_mathml_graphic_device \
  -I$(top_srcdir)/src/view \
  $(NULL)
