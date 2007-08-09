
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
noinst_PROGRAMS += test_rendering
endif
if COND_LIBXML2_READER
noinst_PROGRAMS += test_loading_reader
endif

noinst_HEADERS = guiGTK.h

mathmlviewer_static_SOURCES = main.cc aux.cc guiGTK.c
mathmlviewer_static_LDFLAGS = -no-install -static
mathmlviewer_static_LDADD = \
  $(POPT_LIBS) \
  $(DOM_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_gmetadom.la \
  $(T1_LIBS) \
  $(T1_LIBS_X) \
  $(NULL)

mathmlviewer_SOURCES = main.cc aux.cc guiGTK.c
mathmlviewer_LDADD = \
  $(POPT_LIBS) \
  $(DOM_LIBS) \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(T1_LIBS_X) \
  $(top_builddir)/src/widget/libgtkmathview_gmetadom.la \
  $(NULL)

test_embedding_SOURCES = test_embedding.c dummy.cc
test_embedding_LDFLAGS = -no-install -static
test_embedding_LDADD = \
  -lstdc++ \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_libxml2.la \
  $(T1_LIBS) \
  $(T1_LIBS_X) \
  $(NULL)

test_loading_SOURCES = test_loading.c dummy.cc
test_loading_LDFLASG = -no-install -static
test_loading_LDADD = \
  -lstdc++ \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(T1_LIBS_X) \
  $(top_builddir)/src/widget/libgtkmathview_libxml2.la \
  $(NULL)

test_building_SOURCES = test_building.cc
test_building_LDFLASG = -no-install -static
test_building_LDADD = \
  -lstdc++ \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(T1_LIBS_X) \
  $(top_builddir)/src/widget/libgtkmathview_libxml2.la \
  $(NULL)

test_rendering_SOURCES = test_rendering.cc
test_rendering_LDFLASG = -no-install -static
test_rendering_LDADD = \
  -lstdc++ \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(T1_LIBS_X) \
  $(GLIB_LIBS) \
  $(top_builddir)/src/backend/gtk/libmathview_backend_gtk.la \
  $(top_builddir)/src/view/libmathview_frontend_libxml2.la \
  $(NULL)

test_loading_reader_SOURCES = test_loading_reader.c
test_loading_reader_LDFLASG = -no-install -static
test_loading_reader_LDADD = \
  -lstdc++ \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(T1_LIBS_X) \
  $(top_builddir)/src/widget/libgtkmathview_libxml2_reader.la \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto/ \
  -I$(top_srcdir)/src/common/ \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/frontend/gmetadom/ \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/widget \
  -I$(top_srcdir)/src/view \
  -I$(top_srcdir)/src/backend/common/ \
  -I$(top_srcdir)/src/backend/gtk/ \
  $(POPT_CFLAGS) \
  $(GTK_CFLAGS) \
  $(XML_CFLAGS) \
  $(DOM_CFLAGS) \
  $(XML_CFLAGS) \
  $(NULL)

