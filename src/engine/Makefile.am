
if COND_BOXML
MAYBE_BOXML_SUBDIRS = boxml adapters
endif

SUBDIRS = common mathml $(MAYBE_BOXML_SUBDIRS)

noinst_LTLIBRARIES = libengine.la

if COND_BOXML
MAYBE_BOXML_LIBADD = \
  $(top_builddir)/src/engine/boxml/libboxml.la \
  $(top_builddir)/src/engine/adapters/libadapters.la
endif

libengine_la_LIBADD = \
  $(top_builddir)/src/engine/mathml/libmathml.la \
  $(MAYBE_BOXML_LIBADD) \
  $(top_builddir)/src/engine/common/libengine_common.la

libengine_la_SOURCES = 
