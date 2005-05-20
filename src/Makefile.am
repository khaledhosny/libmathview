
NULL =

SUBDIRS = common engine backend . frontend view widget

lib_LTLIBRARIES = libmathview.la

libmathview_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_la_SOURCES = $(NULL)

libmathview_la_LIBADD = \
  $(top_builddir)/src/backend/libmathview_backend.la \
  $(top_builddir)/src/engine/libengine.la \
  $(top_builddir)/src/common/libcommon.la \
  $(NULL)

