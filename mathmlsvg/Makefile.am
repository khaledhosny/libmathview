
NULL =

bin_PROGRAMS = $(NULL)
if COND_LIBXML2
bin_PROGRAMS += mathmlsvg svgmathmlsvg
endif

mathmlsvg_SOURCES = \
  SVG_libxml2_StreamRenderingContext.cc \
  SVG_libxml2_StreamRenderingContext.hh \
  main.cc \
  $(NULL)

mathmlsvg_LDADD = \
  $(GLIB_LIBS) \
  $(top_builddir)/src/backend/svg/libmathview_backend_svg.la \
  $(top_builddir)/src/view/libmathview_frontend_libxml2.la \
  $(top_builddir)/src/libmathview.la \
  $(NULL)

svgmathmlsvg_SOURCES = \
  SVG_libxml2_StreamRenderingContext.cc \
  SVG_libxml2_StreamRenderingContext.hh \
  SVG_EvalRenderingContext.cc \
  SVG_EvalRenderingContext.hh \
  Fragment.cc \
  Fragment.hh \
  Location.cc \
  Location.hh \
  SMS.cc \
  SMS.hh \
  $(NULL)

INCLUDES = \
  $(XML_CFLAGS) \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/libxml2 \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/backend/common \
  -I$(top_srcdir)/src/backend/svg \
  -I$(top_srcdir)/src/view \
  $(NULL)

