
NULL =

bin_PROGRAMS = $(NULL)
if COND_LIBXML2
bin_PROGRAMS += mathmlsvg
endif

mathmlsvg_SOURCES = \
  Fragment.cc \
  Fragment.hh \
  Location.cc \
  Location.hh \
  MathView.hh \
  Model.cc \
  Model.hh \
  Scanner.cc \
  Scanner.hh \
  SMS.cc \
  SMS.hh \
  SVG_EvalRenderingContext.cc \
  SVG_EvalRenderingContext.hh \
  SVG_libxml2_StreamRenderingContext.cc \
  SVG_libxml2_StreamRenderingContext.hh \
  main.cc \
  $(NULL)

mathmlsvg_LDADD = \
  $(POPT_LIBS) \
  $(GLIB_LIBS) \
  $(top_builddir)/src/backend/svg/libmathview_backend_svg.la \
  $(top_builddir)/src/view/libmathview_frontend_libxml2.la \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/libxml2 \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/backend/common \
  -I$(top_srcdir)/src/backend/svg \
  -I$(top_srcdir)/src/view \
  $(POPT_CFLAGS) \
  $(GLIB_CFLAGS) \
  $(XML_CFLAGS) \
  $(NULL)

