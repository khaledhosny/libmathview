
DOM_CFLAGS = @DOM_CFLAGS@
DOM_LIBS = @DOM_LIBS@

bin_PROGRAMS =
if COND_GMETADOM
bin_PROGRAMS += mathmlviewer
endif

noinst_PROGRAMS = 
if COND_GMETADOM
noinst_PROGRAMS += mathmlviewer.debug
endif
noinst_PROGRAMS += testembedding

noinst_HEADERS = guiGTK.h

mathmlviewer_SOURCES = main.cc aux.cc guiGTK.c

mathmlviewer_LDADD = \
  $(DOM_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_gmetadom.la

mathmlviewer_debug_SOURCES = main.cc aux.cc guiGTK.c

mathmlviewer_debug_LDADD = \
  $(DOM_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_gmetadom.la

testembedding_SOURCES = testembedding.c

testembedding_LDADD = \
  -lstdc++ \
  $(XML_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/widget/libgtkmathview_libxml2.la

INCLUDES = \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/auto/ \
  -I$(top_srcdir)/src/common/ \
  -I$(top_srcdir)/src/frontend/gmetadom/ \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/widget \
  -I$(top_srcdir)/src/formatter/areamodel/

