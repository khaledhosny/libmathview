
DOM_CFLAGS = @DOM_CFLAGS@
DOM_LIBS = @DOM_LIBS@

#bin_PROGRAMS = mathmlviewer
noinst_PROGRAMS = mathmlviewer.debug

noinst_HEADERS = guiGTK.h

#mathmlviewer_SOURCES = main.cc aux.cc guiGTK.c

#mathmlviewer_LDADD = \
  #$(DOM_LIBS) \
  #$(GTK_LIBS) \
  #$(top_builddir)/src/libgtkmathview.la

mathmlviewer_debug_SOURCES = main.cc aux.cc guiGTK.c

mathmlviewer_debug_LDADD = \
  $(DOM_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/libgtkmathview.la

INCLUDES = \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  -I$(top_builddir) \
  -I$(top_srcdir)/src/ \
  -I$(top_srcdir)/src/formatter/basictypes/ \
  -I$(top_srcdir)/src/formatter/areamodel/ \
  -I$(top_srcdir)/auto/

