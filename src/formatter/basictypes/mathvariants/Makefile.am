
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

BUILT_SOURCES = $(AUTO_CC)

CLEANFILES = $(AUTO_CC)

noinst_LTLIBRARIES = libmathvariants.la

libmathvariants_la_SOURCES = \
  $(AUTO_CC) \
  MathVariantMap.cc

libmathvariants_la_LIBADD = \
  $(DOM_LIBS)

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/formatter/basictypes

%_gen.cc : $(srcdir)/xml/%.xml extract.xsl
	$(XSLTPROC) --novalid extract.xsl $< >$@

%.cc : %_gen.cc variant.top variant.bot
	cat variant.top >$@
	$(XSLTPROC) --novalid --param temp "false()" extract.xsl $(<:%_gen.cc=$(srcdir)/xml/%.xml) >>$@
	$(CC) $(DOM_CFLAGS) $(DOM_LIBS) $<
	echo "DOM::Char32 " >>$@
	echo -n "map_$@_variant" | cut -d '.' -f1 | tr "-" "_" >>$@
	echo "(DOM::Char16 ch)" >>$@
	echo "{" >>$@
	./a.out >>$@
	echo "  return ch;" >>$@
	echo "}" >>$@
	rm -f a.out
	cat variant.bot >>$@

