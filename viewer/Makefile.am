
bin_PROGRAMS = mathmlviewer
noinst_PROGRAMS = mathmlviewer.debug

noinst_HEADERS = guiGTK.h

mathmlviewer_SOURCES = main.cc guiGTK.c
mathmlviewer_LDADD = -lgtkmathview
mathmlviewer_LDFLAGS = -L../src/.libs/

mathmlviewer_debug_SOURCES = main.cc guiGTK.c
mathmlviewer_debug_LDADD = ../src/.libs/libgtkmathview.a
mathmlviewer_debug_LDFLAGS = -L../src/.libs/

INCLUDES = -I$(srcdir)/../src/ -I$(srcdir)/../auto/
