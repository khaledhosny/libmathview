
NULL =

if COND_CUSTOM_READER
if COND_LIBXML2_READER
MAYBE_CUSTOM_READER = libgtkmathview_custom_reader.la
else
MAYBE_CUSTOM_READER = $(NULL)
endif
else
MAYBE_CUSTOM_READER = $(NULL)
endif

if COND_LIBXML2_READER
MAYBE_LIBXML2_READER = libgtkmathview_libxml2_reader.la
else
MAYBE_LIBXML2_READER = $(NULL)
endif

if COND_LIBXML2
MAYBE_LIBXML2 = libgtkmathview_libxml2.la
else
MAYBE_LIBXML2 = $(NULL)
endif

if COND_GMETADOM
MAYBE_GMETADOM = libgtkmathview_gmetadom.la
else
MAYBE_GMETADOM = $(NULL)
endif

lib_LTLIBRARIES = $(MAYBE_CUSTOM_READER) $(MAYBE_LIBXML2_READER) $(MAYBE_LIBXML2) $(MAYBE_GMETADOM)

libgtkmathview_custom_reader_la_LIBADD = \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(T1_LIBS_X) \
  $(top_builddir)/src/backend/gtk/libmathview_backend_gtk.la \
  $(top_builddir)/src/view/libmathview_frontend_custom_reader.la \
  $(NULL)

libgtkmathview_libxml2_reader_la_LIBADD = \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(T1_LIBS_X) \
  $(top_builddir)/src/backend/gtk/libmathview_backend_gtk.la \
  $(top_builddir)/src/view/libmathview_frontend_libxml2_reader.la \
  $(NULL)

libgtkmathview_libxml2_la_LIBADD = \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(T1_LIBS_X) \
  $(top_builddir)/src/backend/gtk/libmathview_backend_gtk.la \
  $(top_builddir)/src/view/libmathview_frontend_libxml2.la \
  $(NULL)

libgtkmathview_gmetadom_la_LIBADD = \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(T1_LIBS_X) \
  $(top_builddir)/src/backend/gtk/libmathview_backend_gtk.la \
  $(top_builddir)/src/view/libmathview_frontend_gmetadom.la \
  $(NULL)

libgtkmathview_custom_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libgtkmathview_libxml2_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libgtkmathview_libxml2_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libgtkmathview_gmetadom_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libgtkmathview_custom_reader_la_SOURCES = gtkmathview_custom_reader.cc
libgtkmathview_libxml2_reader_la_SOURCES = gtkmathview_libxml2_reader.cc
libgtkmathview_libxml2_la_SOURCES = gtkmathview_libxml2.cc
libgtkmathview_gmetadom_la_SOURCES = gtkmathview_gmetadom.cc

libgtkmathview_custom_reader_la_CPPFLAGS = -DGMV_Widget_DLL
libgtkmathview_libxml2_reader_la_CPPFLAGS = -DGMV_Widget_DLL
libgtkmathview_libxml2_la_CPPFLAGS = -DGMV_Widget_DLL
libgtkmathview_gmetadom_la_CPPFLAGS = -DGMV_Widget_DLL

EXTRA_DIST = \
  gtkmathview_common.cc \
  gtkmathview_decorators_common.cc \
  $(NULL)

gtkdir = $(includedir)/$(PACKAGE)/gtk
gtk_HEADERS = \
  gtkmathview.h \
  gtkmathview_decorators.h \
  gtkmathview_common.h \
  gtkmathview_decorators_common.h \
  gtkmathview_custom_reader.h \
  gtkmathview_libxml2_reader.h \
  gtkmathview_libxml2.h \
  gtkmathview_gmetadom.h

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/custom_reader \
  -I$(top_srcdir)/src/frontend/libxml2_reader \
  -I$(top_srcdir)/src/frontend/libxml2 \
  -I$(top_srcdir)/src/frontend/gmetadom \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/backend/common \
  -I$(top_srcdir)/src/backend/gtk \
  -I$(top_srcdir)/src/view \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  $(XML_CFLAGS) \
  $(NULL)

