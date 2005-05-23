
NULL =

if COND_GTK
MAYBE_GTK_SUBDIRS = widget
else
MAYBE_GTK_SUBDIRS = $(NULL)
endif

SUBDIRS = common engine backend frontend . view $(MAYBE_GTK_SUBDIRS)

if COND_BOXML
MAYBE_BOXML_LIBADD = \
  $(top_builddir)/src/engine/boxml/libboxml.la \
  $(top_builddir)/src/engine/adapters/libadapters.la
else
MAYBE_BOXML_LIBADD = $(NULL)
endif

lib_LTLIBRARIES = libmathview.la

libmathview_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_la_SOURCES = $(NULL)

libmathview_la_LIBADD = \
  $(top_builddir)/src/backend/common/libbackend_common.la \
  $(top_builddir)/src/frontend/common/libfrontend_common.la \
  $(top_builddir)/src/engine/mathml/libmathml.la \
  $(MAYBE_BOXML_LIBADD) \
  $(top_builddir)/src/engine/common/libengine_common.la \
  $(top_builddir)/src/common/libcommon.la \
  $(NULL)

