
NULL =

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

libmathvariants_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common

if COND_XSLTPROC
XSLTPROC = xsltproc

%_gen.cc : $(srcdir)/xml/%.xml $(srcdir)/extract.xsl
	$(AM_V_GEN)$(XSLTPROC) --novalid $(srcdir)/extract.xsl $< >$@

%.cc : %_gen.cc $(srcdir)/variant.top $(srcdir)/variant.bot
	$(AM_V_GEN)
	$(AM_V_at)cat $(srcdir)/variant.top >$@
	$(AM_V_at)$(XSLTPROC) --novalid --param temp "false()" $(srcdir)/extract.xsl $(<:%_gen.cc=$(srcdir)/xml/%.xml) >>$@
	$(AM_V_at)$(CXX) -o $(@:%.cc=%) -I$(top_builddir)/auto $<
	$(AM_V_at)echo "Char32 " >>$@
	$(AM_V_at)basename map_variant_$@ .cc | tr "-" "_" >>$@
	$(AM_V_at)echo "(Char32 ch)" >>$@
	$(AM_V_at)echo "{" >>$@
	$(AM_V_at)./$(@:%.cc=%) >>$@
	$(AM_V_at)echo "  return ch;" >>$@
	$(AM_V_at)echo "}" >>$@
	$(AM_V_at)rm -f $(@:%.cc=%)
	$(AM_V_at)cat $(srcdir)/variant.bot >>$@
endif

EXTRA_DIST += \
  xml/bold-fraktur.xml \
  xml/bold-italic.xml \
  xml/bold-sans-serif.xml \
  xml/bold-script.xml \
  xml/bold.xml \
  xml/double-struck.xml \
  xml/fraktur.xml \
  xml/italic.xml \
  xml/monospace.xml \
  xml/sans-serif-bold-italic.xml \
  xml/sans-serif-italic.xml \
  xml/sans-serif.xml \
  xml/script.xml


-include $(top_srcdir)/git.mk
