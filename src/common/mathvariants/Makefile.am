
SUBDIRS = xml

XSLTPROC = xsltproc

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
  script.cc

EXTRA_DIST = variant.top variant.bot extract.xsl

BUILT_SOURCES = $(AUTO_CC)

CLEANFILES = $(AUTO_CC)

noinst_LTLIBRARIES = libmathvariants.la

libmathvariants_la_SOURCES = \
  $(AUTO_CC) \
  MathVariantMap.cc

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  MathVariant.hh

noinst_HEADERS = \
  MathVariantMap.hh

INCLUDES = \
  -I$(top_srcdir)/src/common

%_gen.cc : $(srcdir)/xml/%.xml extract.xsl
	$(XSLTPROC) --novalid extract.xsl $< >$@

%.cc : %_gen.cc variant.top variant.bot
	cat variant.top >$@
	$(XSLTPROC) --novalid --param temp "false()" extract.xsl $(<:%_gen.cc=$(srcdir)/xml/%.xml) >>$@
	$(CXX) -I$(top_srcdir)/src/common $<
	echo "Char32 " >>$@
	echo -n "map_variant_$@" | cut -d '.' -f1 | tr "-" "_" >>$@
	echo "(Char32 ch)" >>$@
	echo "{" >>$@
	./a.out >>$@
	echo "  return ch;" >>$@
	echo "}" >>$@
	rm -f a.out
	cat variant.bot >>$@

