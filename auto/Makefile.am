
noinst_PROGRAMS = dumpEntitiesTable

dumpEntitiesTable_SOURCES = dumpEntitiesTable.cc
dumpEntitiesTable_LDADD = @DOM_LIBS@

XSLTPROC = xsltproc

AUTOFILES = \
  token.dec token.def \
  entitiesTable.inc

EXTRA_DIST = \
  list-to-dec.xsl \
  list-to-def.xsl \
  config.dirs.in \
  config.dirs \
  token-list.xml \
  stretchy.list \
  entitiesTable.inc \
  $(AUTOFILES)

# config.dirs should not be distributed but omitting it makes
# distcheck fail.

DISTCLEANFILES = $(AUTOFILES)

noinst_HEADERS = $(AUTOFILES)

SCRIPTDIR = ../scripts
CONFIGDIR = ../config

token.dec: token-list.xml list-to-dec.xsl
	$(XSLTPROC) list-to-dec.xsl $< >$@

token.def: token-list.xml list-to-def.xsl
	$(XSLTPROC) list-to-def.xsl $< >$@

entitiesTable.inc: $(CONFIGDIR)/entities-table.xml dumpEntitiesTable
	./dumpEntitiesTable $< >$@

INCLUDES = \
  @DOM_CFLAGS@ \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/frontend/gmetadom \
  -I$(top_builddir)

