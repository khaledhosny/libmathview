
NULL =

if COND_SVG
MAYBE_SVG_SUBDIRS = svg
else
MAYBE_SVG_SUBDIRS = $(NULL)
endif

SUBDIRS = \
  common \
  gtk \
  $(MAYBE_SVG_SUBDIRS) \
  $(NULL)

noinst_LTLIBRARIES = libmathview_backend.la

if COND_SVG
MAYBE_SVG_LIBADD = $(top_builddir)/src/backend/svg/libbackend_svg.la
else
MAYBE_SVG_LIBADD = $(NULL)
endif

libmathview_backend_la_LIBADD = \
  $(top_builddir)/src/backend/gtk/libmathview_backend_gtk.la \
  $(MAYBE_SVG_LIBADD) \
  $(top_builddir)/src/backend/common/libmathview_backend_common.la \
  $(NULL)

libmathview_backend_la_SOURCES = $(NULL)

