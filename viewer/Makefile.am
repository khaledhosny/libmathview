
NULL =

bin_PROGRAMS = $(NULL)
if HAVE_GTK
if HAVE_LIBXML2
bin_PROGRAMS += mml-view-gtk
endif
endif

noinst_PROGRAMS = $(NULL)
if HAVE_LIBXML2
if HAVE_GTK
noinst_PROGRAMS += test_embedding
noinst_PROGRAMS += test_loading
noinst_PROGRAMS += test_building
noinst_PROGRAMS += test_rendering
endif
if HAVE_CAIRO
bin_PROGRAMS += mml-view
endif
if HAVE_QT
moc_%.cc: %.hh
	$(MOC) $< -o $@
bin_PROGRAMS += mml-view-qt
endif
endif

if HAVE_LIBXML2_READER
if HAVE_GTK
noinst_PROGRAMS += test_loading_reader
endif
endif

if HAVE_QT
mml_view_qt_SOURCES = mml-view-qt.cc Qt_RenderArea.cc moc_Qt_RenderArea.cc
mml_view_qt_LDADD = \
  $(QT_LIBS) \
  $(XML_LIBS) \
  $(GLIB_LIBS) \
  $(top_builddir)/src/libmathview.la \
  $(top_builddir)/src/libmathview_backend_qt.la \
  $(top_builddir)/src/libmathview_frontend_libxml2.la \
  $(NULL)
endif

if HAVE_CAIRO
mml_view_SOURCES = mml-view.cc
mml_view_LDADD = \
  $(XML_LIBS) \
  $(CAIRO_LIBS) \
  $(GLIB_LIBS) \
  $(top_builddir)/src/libmathview.la \
  $(top_builddir)/src/libmathview_backend_cairo.la \
  $(top_builddir)/src/libmathview_frontend_libxml2.la \
  $(NULL)
endif

if HAVE_GTK
noinst_HEADERS = Gtk_Window.h

mml_view_gtk_SOURCES = mml-view-gtk.cc Gtk_Window.c


mml_view_gtk_LDADD = \
  $(GTK_LIBS) \
  $(top_builddir)/src/libgtkmathview_libxml2.la \
  $(NULL)

test_embedding_SOURCES = test_embedding.c dummy.cc
test_embedding_LDFLAGS = -no-install
test_embedding_LDADD = \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/libgtkmathview_libxml2.la \
  $(NULL)

test_loading_SOURCES = test_loading.c dummy.cc
test_loading_LDFLAGS = -no-install
test_loading_LDADD = \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/libgtkmathview_libxml2.la \
  $(NULL)

test_building_SOURCES = test_building.cc
test_building_LDFLAGS = -no-install
test_building_LDADD = \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/libgtkmathview_libxml2.la \
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
  $(top_builddir)/src/libgtkmathview_libxml2_reader.la \
  $(NULL)
endif

AM_CPPFLAGS = \
  -I$(top_builddir)/auto/ \
  -I$(top_srcdir)/src/common/ \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/widget \
  -I$(top_srcdir)/src/view \
  -I$(top_srcdir)/src/backend/common/ \
  -I$(top_srcdir)/src/backend/cairo/ \
  -I$(top_srcdir)/src/backend/qt/ \
  $(CAIRO_CFLAGS) \
  $(QT_CFLAGS) -fPIC \
  $(GLIB_CFLAGS) \
  $(GTK_CFLAGS) \
  $(XML_CFLAGS) \
  $(NULL)


-include $(top_srcdir)/git.mk
