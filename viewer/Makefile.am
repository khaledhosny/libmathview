
NULL =

bin_PROGRAMS = $(NULL)
if COND_GMETADOM
bin_PROGRAMS += mathmlviewer
endif

noinst_PROGRAMS = $(NULL)
if COND_LIBXML2
noinst_PROGRAMS += test_embedding
noinst_PROGRAMS += test_loading
noinst_PROGRAMS += test_building
noinst_PROGRAMS += test_rendering
bin_PROGRAMS += mml-view
endif
if COND_LIBXML2_READER
noinst_PROGRAMS += test_loading_reader
endif

noinst_HEADERS = guiGTK.h

mathmlviewer_SOURCES = main.cc aux.cc guiGTK.c
mathmlviewer_LDADD = \
  $(DOM_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_gmetadom.la \
  $(NULL)

mml_view_SOURCES = mml-view.cc
mml_view_LDADD = \
  $(XML_LIBS) \
  $(CAIRO_LIBS) \
  $(PANGO_LIBS) \
  $(GLIB_LIBS) \
  $(top_builddir)/src/libmathview.la \
  $(top_builddir)/src/libmathview_backend_cairo.la \
  $(top_builddir)/src/libmathview_frontend_libxml2.la \
  $(NULL)

test_embedding_SOURCES = test_embedding.c dummy.cc
test_embedding_LDFLAGS = -no-install
test_embedding_LDADD = \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_libxml2.la \
  $(NULL)

test_loading_SOURCES = test_loading.c dummy.cc
test_loading_LDFLAGS = -no-install
test_loading_LDADD = \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_libxml2.la \
  $(NULL)

test_building_SOURCES = test_building.cc
test_building_LDFLAGS = -no-install
test_building_LDADD = \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_libxml2.la \
  $(NULL)

test_rendering_SOURCES = test_rendering.cc
test_rendering_LDFLAGS = -no-install
test_rendering_LDADD = \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(GLIB_LIBS) \
  $(top_builddir)/src/libmathview.la \
  $(top_builddir)/src/libmathview_backend_cairo.la \
  $(top_builddir)/src/libmathview_frontend_libxml2.la \
  $(NULL)

test_loading_reader_SOURCES = test_loading_reader.c
test_loading_reader_LDFLAGS = -no-install
test_loading_reader_LDADD = \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_libxml2_reader.la \
  $(NULL)

AM_CPPFLAGS = \
  -I$(top_builddir)/auto/ \
  -I$(top_srcdir)/src/common/ \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/frontend/gmetadom/ \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/widget \
  -I$(top_srcdir)/src/view \
  -I$(top_srcdir)/src/backend/common/ \
  -I$(top_srcdir)/src/backend/cairo/ \
  $(GTK_CFLAGS) \
  $(XML_CFLAGS) \
  $(DOM_CFLAGS) \
  $(XML_CFLAGS) \
  $(NULL)


-include $(top_srcdir)/git.mk
