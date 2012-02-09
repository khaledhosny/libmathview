
NULL =

if COND_GTK
MAYBE_GTK_SUBDIRS = gtk
else
MAYBE_GTK_SUBDIRS = $(NULL)
endif

if COND_SVG
MAYBE_SVG_SUBDIRS = svg
else
MAYBE_SVG_SUBDIRS = $(NULL)
endif

SUBDIRS = \
  common \
  $(MAYBE_GTK_SUBDIRS) \
  $(MAYBE_SVG_SUBDIRS) \
  $(NULL)

-include $(top_srcdir)/git.mk
