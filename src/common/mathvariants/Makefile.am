
NULL =

SUBDIRS = xml

AUTO_CC = \
  bold-fraktur.cc \
  bold-italic.cc \
  bold-sans-serif.cc \
  bold-script.cc \
  bold.cc \
  double-struck.cc \
  fraktur.cc \
  italic.cc \
  monospace.cc \
  sans-serif-bold-italic.cc \
  sans-serif-italic.cc \
  sans-serif.cc \
  script.cc \
  $(NULL)

EXTRA_DIST = variant.top variant.bot extract.xsl

BUILT_SOURCES = $(AUTO_CC)

CLEANFILES = $(AUTO_CC)

noinst_LTLIBRARIES = libmathvariants.la

libmathvariants_la_CPPFLAGS = -DGMV_MathView_DLL
libmathvariants_la_SOURCES = \
  $(AUTO_CC) \
  MathVariantMap.cc \
  $(NULL)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  MathVariant.hh \
  MathVariantMap.hh \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common

if COND_XSLTPROC
XSLTPROC = xsltproc

%_gen.cc : $(srcdir)/xml/%.xml $(srcdir)/extract.xsl
	$(XSLTPROC) --novalid $(srcdir)/extract.xsl $< >$@

%.cc : %_gen.cc $(srcdir)/variant.top $(srcdir)/variant.bot
	cat $(srcdir)/variant.top >$@
	$(XSLTPROC) --novalid --param temp "false()" $(srcdir)/extract.xsl $(<:%_gen.cc=$(srcdir)/xml/%.xml) >>$@
	$(CXX) -o $(@:%.cc=%) -I$(top_builddir)/auto $<
	echo "Char32 " >>$@
	basename map_variant_$@ .cc | tr "-" "_" >>$@
	echo "(Char32 ch)" >>$@
	echo "{" >>$@
	./$(@:%.cc=%) >>$@
	echo "  return ch;" >>$@
	echo "}" >>$@
	rm -f $(@:%.cc=%)
	cat $(srcdir)/variant.bot >>$@
endif

