
bin_PROGRAMS = mathml2ps
noinst_PROGRAMS = mathml2ps.debug

mathml2ps_SOURCES = main.cc
mathml2ps_LDADD = -lgtkmathview
mathml2ps_LDFLAGS =  -L../src/.libs

mathml2ps_debug_SOURCES = main.cc
mathml2ps_debug_LDADD = ../src/.libs/libgtkmathview.a

INCLUDES = -I$(srcdir)/../src/ -I$(srcdir)/../auto/
