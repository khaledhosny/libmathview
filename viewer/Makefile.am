
NULL =

bin_PROGRAMS = $(NULL)
if COND_GMETADOM
bin_PROGRAMS += mathmlviewer
endif

noinst_PROGRAMS = $(NULL)
if COND_GMETADOM
if COND_PROFILE
noinst_PROGRAMS += mathmlviewer.static
endif
endif
if COND_LIBXML2
noinst_PROGRAMS += test_embedding
noinst_PROGRAMS += test_loading
noinst_PROGRAMS += test_building
endif
if COND_LIBXML2_READER
noinst_PROGRAMS += test_loading_reader
endif

noinst_HEADERS = guiGTK.h

mathmlviewer_static_SOURCES = main.cc aux.cc guiGTK.c
mathmlviewer_static_LDFLAGS = -no-install -static
mathmlviewer_static_LDADD = \
  $(DOM_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_gmetadom.la \
  $(top_builddir)/src/backend/gtk/libmathview_backend_gtk.la \
  $(top_builddir)/src/view/libmathview_frontend_gmetadom.la \
  $(top_builddir)/src/libmathview.la \
  $(T1_LIBS) \
  $(NULL)

mathmlviewer_SOURCES = main.cc aux.cc guiGTK.c
mathmlviewer_LDADD = \
  $(DOM_LIBS) \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_gmetadom.la \
  $(top_builddir)/src/backend/gtk/libmathview_backend_gtk.la \
  $(top_builddir)/src/view/libmathview_frontend_gmetadom.la \
  $(top_builddir)/src/libmathview.la \
  $(NULL)

test_embedding_SOURCES = test_embedding.c
test_embedding_LDFLAGS = -no-install -static
test_embedding_LDADD = \
  -lstdc++ \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_libxml2.la \
  $(top_builddir)/src/backend/gtk/libmathview_backend_gtk.la \
  $(top_builddir)/src/view/libmathview_frontend_libxml2.la \
  $(top_builddir)/src/libmathview.la \
  $(T1_LIBS) \
  $(NULL)

test_loading_SOURCES = test_loading.c
test_loading_LDFLASG = -no-install -static
test_loading_LDADD = \
  -lstdc++ \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_libxml2.la \
  $(top_builddir)/src/backend/gtk/libmathview_backend_gtk.la \
  $(top_builddir)/src/view/libmathview_frontend_libxml2.la \
  $(top_builddir)/src/libmathview.la \
  $(NULL)

test_building_SOURCES = test_building.cc
test_building_LDFLASG = -no-install -static
test_building_LDADD = \
  -lstdc++ \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_libxml2.la \
  $(top_builddir)/src/backend/gtk/libmathview_backend_gtk.la \
  $(top_builddir)/src/view/libmathview_frontend_libxml2.la \
  $(top_builddir)/src/libmathview.la \
  $(NULL)

test_loading_reader_SOURCES = test_loading_reader.c
test_loading_reader_LDFLASG = -no-install -static
test_loading_reader_LDADD = \
  -lstdc++ \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_libxml2_reader.la \
  $(top_builddir)/src/backend/gtk/libmathview_backend_gtk.la \
  $(top_builddir)/src/view/libmathview_frontend_libxml2_reader.la \
  $(top_builddir)/src/libmathview.la \
  $(NULL)

INCLUDES = \
  $(GTK_CFLAGS) \
  $(XML_CFLAGS) \
  $(DOM_CFLAGS) \
  $(XML_CFLAGS) \
  -I$(top_builddir)/auto/ \
  -I$(top_srcdir)/src/common/ \
  -I$(top_srcdir)/src/frontend/gmetadom/ \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/widget \
  -I$(top_srcdir)/src/backend/common/ \
  $(NULL)

