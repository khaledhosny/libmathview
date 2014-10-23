
NULL =

AUTOFILES = \
  entitiesTable.inc \
  operatorDictionary.cc \
  operatorDictionary.hh \
  token.dec \
  token.def \
  $(NULL)

EXTRA_DIST = \
  unicode.xml \
  list-to-dec.xsl \
  list-to-def.xsl \
  token-list.xml \
  token.dec \
  token.def \
  $(NULL)

DISTCLEANFILES = \
  entitiesTable.inc \
  operatorDictionary.cc \
  operatorDictionary.hh \
  $(NULL)

MAINTAINERCLEANFILES = token.dec token.def

mathviewdir = $(pkgincludedir)/MathView

noinst_HEADERS = $(AUTOFILES)

SCRIPTDIR = $(top_srcdir)/scripts
CONFIGDIR = $(top_srcdir)/config

entitiesTable.inc: unicode.xml $(SCRIPTDIR)/dumpEntitiesTable
	$(AM_V_GEN)$(PYTHON) $(SCRIPTDIR)/dumpEntitiesTable $< > $@

operatorDictionary.hh: unicode.xml $(SCRIPTDIR)/dumpOperatorDictionary
	$(AM_V_GEN)$(PYTHON) $(SCRIPTDIR)/dumpOperatorDictionary $< h > $@

operatorDictionary.cc: unicode.xml $(SCRIPTDIR)/dumpOperatorDictionary
	$(AM_V_GEN)$(PYTHON) $(SCRIPTDIR)/dumpOperatorDictionary $< c > $@

token.dec: token-list.xml list-to-dec.xsl
	$(AM_V_GEN)$(XSLTPROC) $(srcdir)/list-to-dec.xsl $< >$@

token.def: token-list.xml list-to-def.xsl
	$(AM_V_GEN)$(XSLTPROC) $(srcdir)/list-to-def.xsl $< >$@

-include $(top_srcdir)/git.mk
