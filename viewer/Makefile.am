
bin_PROGRAMS = mathmlviewer
noinst_PROGRAMS = mathmlviewer.debug

noinst_HEADERS = guiGTK.h

mathmlviewer_SOURCES = main.cc guiGTK.c
mathmlviewer_LDADD = $(top_builddir)/src/libgtkmathview.la

mathmlviewer_debug_SOURCES = main.cc guiGTK.c
mathmlviewer_debug_LDADD = $(top_builddir)/src/libgtkmathview.la

INCLUDES = -I$(srcdir)/../src/ -I$(srcdir)/../auto/
