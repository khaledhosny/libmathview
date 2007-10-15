
NULL =

noinst_PROGRAMS =
if COND_GMETADOM
noinst_PROGRAMS += dumpEntitiesTable

entitiesTable.inc: $(CONFIGDIR)/entities-table.xml dumpEntitiesTable
	./dumpEntitiesTable $< >$@

else
entitiesTable.inc:
	cp $(top_srcdir)/auto/entitiesTable.inc_ entitiesTable.inc
endif

dumpEntitiesTable_SOURCES = dumpEntitiesTable.cc
dumpEntitiesTable_LDADD = @DOM_LIBS@

AUTOFILES = \
  token.dec \
  token.def \
  config.dirs \
  entitiesTable.inc \
  $(NULL)

EXTRA_DIST = \
  list-to-dec.xsl \
  list-to-def.xsl \
  token-list.xml \
  stretchy.list \
  Char.hh.in.in \
  Char.hh.in \
  token.dec \
  token.def \
  entitiesTable.inc_ \
  $(NULL)

DISTCLEANFILES = config.dirs entitiesTable.inc Char.hh
MAINTAINERCLEANFILES = token.dec token.def

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  Char.hh

noinst_HEADERS = $(AUTOFILES)

SCRIPTDIR = ../scripts
CONFIGDIR = ../config

if COND_XSLTPROC
XSLTPROC = xsltproc

token.dec: token-list.xml list-to-dec.xsl
	$(XSLTPROC) $(srcdir)/list-to-dec.xsl $< >$@

token.def: token-list.xml list-to-def.xsl
	$(XSLTPROC) $(srcdir)/list-to-def.xsl $< >$@
endif

.PHONY: config.dirs
config.dirs:
	( \
	echo "#define PKGDATADIR \"$(pkgdatadir)\"" >$@.maybe; \
	echo "#define PKGSYSCONFDIR \"$(sysconfdir)/@PACKAGE@\"" >>$@.maybe; \
	if test ! \( -e $@ \); then \
	  mv $@.maybe $@; \
        elif ! diff $@.maybe $@ >/dev/null; then \
	  mv $@.maybe $@; \
	else \
	  rm $@.maybe; \
	fi \
	)

Char.hh: $(top_builddir)/auto/Char.hh.in
	( \
	if test ! \( -e $@ \); then \
	  cp $< $@; \
        elif ! diff $< $@ >/dev/null; then \
	  cp $< $@; \
	fi \
	)

INCLUDES = \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/frontend/gmetadom \
  -I$(top_builddir) \
  $(DOM_CFLAGS) \
  $(NULL)
