
noinst_PROGRAMS =
if COND_GMETADOM
noinst_PROGRAMS += dumpEntitiesTable
endif

dumpEntitiesTable_SOURCES = dumpEntitiesTable.cc
dumpEntitiesTable_LDADD = @DOM_LIBS@

XSLTPROC = xsltproc

AUTOFILES = \
  token.dec \
  token.def

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

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  token.dec

noinst_HEADERS = \
  token.def \
  entitiesTable.inc

SCRIPTDIR = ../scripts
CONFIGDIR = ../config

token.dec: token-list.xml list-to-dec.xsl
	$(XSLTPROC) list-to-dec.xsl $< >$@

token.def: token-list.xml list-to-def.xsl
	$(XSLTPROC) list-to-def.xsl $< >$@

update-entities: $(CONFIGDIR)/entities-table.xml dumpEntitiesTable
	./dumpEntitiesTable $< >$@

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/frontend/gmetadom \
  -I$(top_builddir)

