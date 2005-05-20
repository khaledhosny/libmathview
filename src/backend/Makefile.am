
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

noinst_LTLIBRARIES = libbackend.la

if COND_SVG
MAYBE_SVG_LIBADD = $(top_builddir)/src/backend/svg/libbackend_svg.la
else
MAYBE_SVG_LIBADD = $(NULL)
endif

libbackend_la_LIBADD = \
  $(top_builddir)/src/backend/gtk/libbackend_gtk.la \
  $(top_builddir)/src/backend/common/libbackend_common.la \
  $(MAYBE_SVG_LIBADD) \
  $(NULL)

libbackend_la_SOURCES =

