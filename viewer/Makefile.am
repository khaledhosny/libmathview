
bin_PROGRAMS = mathmlviewer
noinst_PROGRAMS = mathmlviewer.debug

noinst_HEADERS = guiGTK.h

mathmlviewer_SOURCES = main.cc guiGTK.c

mathmlviewer_LDADD = $(GTK_LIBS) $(top_builddir)/src/libgtkmathview.la

mathmlviewer_debug_SOURCES = main.cc guiGTK.c

mathmlviewer_debug_LDADD = $(GTK_LIBS) $(top_builddir)/src/libgtkmathview.la

INCLUDES = \
  $(GTK_CFLAGS) \
  -I$(top_srcdir)/src/ \
  -I$(top_srcdir)/auto/

