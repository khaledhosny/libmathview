
NULL =

if COND_GTK
MAYBE_GTK_SUBDIRS = gtk
else
MAYBE_GTK_SUBDIRS = $(NULL)
endif

SUBDIRS = \
  common \
  $(MAYBE_GTK_SUBDIRS) \
  $(NULL)

-include $(top_srcdir)/git.mk
