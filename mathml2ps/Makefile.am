
DOM_CFLAGS = @DOM_CFLAGS@
DOM_LIBS = @DOM_LIBS@

bin_PROGRAMS = mathml2ps
noinst_PROGRAMS = mathml2ps.debug

mathml2ps_SOURCES = main.cc
mathml2ps_LDADD = \
  $(DOM_LIBS) \
  $(top_builddir)/src/libgtkmathview.la

mathml2ps_debug_SOURCES = main.cc
mathml2ps_debug_LDADD = \
  $(DOM_LIBS) \
  $(top_builddir)/src/libgtkmathview.la

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/ \
  -I$(top_srcdir)/auto/

