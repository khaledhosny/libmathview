
NULL =

if COND_GTK
MAYBE_GTK_SUBDIRS = widget
else
MAYBE_GTK_SUBDIRS = $(NULL)
endif

SUBDIRS = common engine backend frontend . view $(MAYBE_GTK_SUBDIRS)

lib_LTLIBRARIES = libmathview.la

libmathview_la_CPPFLAGS = -DGMV_MathView_DLL
libmathview_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined

libmathview_la_SOURCES = dummy.cc

libmathview_la_LIBADD = \
  $(top_builddir)/src/backend/common/libbackend_common.la \
  $(top_builddir)/src/frontend/common/libfrontend_common.la \
  $(top_builddir)/src/engine/mathml/libmathml.la \
  $(top_builddir)/src/engine/common/libengine_common.la \
  $(top_builddir)/src/common/libcommon.la \
  -lstdc++ \
  $(NULL)

-include $(top_srcdir)/git.mk
