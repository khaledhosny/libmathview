
bin_PROGRAMS = viewer
noinst_PROGRAMS = viewer.debug

noinst_HEADERS = guiGTK.h

viewer_SOURCES = main.cc guiGTK.c
viewer_LDADD = -lgtkmathview
viewer_LDFLAGS = -L../src/.libs/

viewer_debug_SOURCES = main.cc guiGTK.c
viewer_debug_LDADD = ../src/.libs/libgtkmathview.a
viewer_debug_LDFLAGS = -L../src/.libs/

INCLUDES = -I$(srcdir)/../src/ -I$(srcdir)/../auto/
