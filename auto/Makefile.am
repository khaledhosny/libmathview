
NULL =

noinst_PROGRAMS =
if COND_GMETADOM
noinst_PROGRAMS += dumpEntitiesTable

entitiesTable.inc: $(CONFIGDIR)/entities-table.xml dumpEntitiesTable
	./dumpEntitiesTable $< >$@

else
entitiesTable.inc:
	cp entitiesTable.inc_ entitiesTable.inc
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
  Char.hh.in \
  token.dec \
  token.def \
  entitiesTable.inc_ \
  $(NULL)

DISTCLEANFILES = $(AUTOFILES)

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

config.dirs: Makefile
	echo "#define PKGDATADIR \"$(pkgdatadir)\"" >$@
	echo "#define PKGSYSCONFDIR \"$(sysconfdir)/@PACKAGE@\"" >>$@

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/frontend/gmetadom \
  -I$(top_builddir)
