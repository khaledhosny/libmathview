
NULL =

AUTOFILES = \
  entitiesTable.inc \
  operatorDictionary.cc \
  token.dec \
  token.def \
  $(NULL)

EXTRA_DIST = \
  unicode.xml \
  token-list.xml \
  token.dec \
  token.def \
  $(NULL)

DISTCLEANFILES = \
  entitiesTable.inc \
  operatorDictionary.cc \
  $(NULL)

MAINTAINERCLEANFILES = token.dec token.def

mathviewdir = $(pkgincludedir)/MathView

noinst_HEADERS = $(AUTOFILES)

SCRIPTDIR = $(top_srcdir)/scripts
CONFIGDIR = $(top_srcdir)/config

entitiesTable.inc: unicode.xml $(SCRIPTDIR)/dumpEntitiesTable
	$(AM_V_GEN)$(PYTHON) $(SCRIPTDIR)/dumpEntitiesTable $< > $@

operatorDictionary.cc: unicode.xml $(SCRIPTDIR)/dumpOperatorDictionary
	$(AM_V_GEN)$(PYTHON) $(SCRIPTDIR)/dumpOperatorDictionary $< > $@

token.dec: token-list.xml $(SCRIPTDIR)/dumpTokenList
	$(AM_V_GEN)$(PYTHON) $(SCRIPTDIR)/dumpTokenList $< h > $@

token.def: token-list.xml $(SCRIPTDIR)/dumpTokenList
	$(AM_V_GEN)$(PYTHON) $(SCRIPTDIR)/dumpTokenList $< c > $@

-include $(top_srcdir)/git.mk
