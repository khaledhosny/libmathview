
SUBDIRS = common engine formatter . frontend view widget

lib_LTLIBRARIES = libmathview.la

libmathview_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_la_SOURCES =

libmathview_la_LIBADD = \
  $(top_builddir)/src/formatter/libformatter.la \
  $(top_builddir)/src/engine/libengine.la \
  $(top_builddir)/src/common/libcommon.la

