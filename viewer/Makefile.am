
DOM_CFLAGS = @DOM_CFLAGS@
DOM_LIBS = @DOM_LIBS@

bin_PROGRAMS = mathmlviewer
noinst_PROGRAMS = mathmlviewer.debug

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

