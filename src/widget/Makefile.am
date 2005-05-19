
NULL =

lib_LTLIBRARIES =

if COND_CUSTOM_READER
if COND_LIBXML2_READER
lib_LTLIBRARIES += libgtkmathview_custom_reader.la
endif
endif

if COND_LIBXML2_READER
lib_LTLIBRARIES += libgtkmathview_libxml2_reader.la
endif

if COND_LIBXML2
lib_LTLIBRARIES += libgtkmathview_libxml2.la
endif

if COND_GMETADOM
lib_LTLIBRARIES += libgtkmathview_gmetadom.la
endif

libgtkmathview_custom_reader_la_LIBADD = \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(top_builddir)/src/view/libmathview_custom_reader.la \
  $(NULL)

libgtkmathview_libxml2_reader_la_LIBADD = \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(top_builddir)/src/view/libmathview_libxml2_reader.la \
  $(NULL)

libgtkmathview_libxml2_la_LIBADD = \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(top_builddir)/src/view/libmathview_libxml2.la \
  $(NULL)

libgtkmathview_gmetadom_la_LIBADD = \
  $(GTK_LIBS) \
  $(T1_LIBS) \
  $(top_builddir)/src/view/libmathview_gmetadom.la \
  $(NULL)

libgtkmathview_custom_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libgtkmathview_libxml2_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libgtkmathview_libxml2_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libgtkmathview_gmetadom_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libgtkmathview_custom_reader_la_SOURCES = gtkmathview_custom_reader.cc
libgtkmathview_libxml2_reader_la_SOURCES = gtkmathview_libxml2_reader.cc
libgtkmathview_libxml2_la_SOURCES = gtkmathview_libxml2.cc
libgtkmathview_gmetadom_la_SOURCES = gtkmathview_gmetadom.cc

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
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  $(XML_CFLAGS) \
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
  $(NULL)

