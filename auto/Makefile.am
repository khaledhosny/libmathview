
noinst_PROGRAMS =
if COND_GMETADOM
noinst_PROGRAMS += dumpEntitiesTable
endif

dumpEntitiesTable_SOURCES = dumpEntitiesTable.cc
dumpEntitiesTable_LDADD = @DOM_LIBS@

AUTOFILES = \
  token.dec \
  token.def

EXTRA_DIST = \
  list-to-dec.xsl \
  list-to-def.xsl \
  token-list.xml \
  stretchy.list \
  entitiesTable.inc \
  Char.hh.in \
  Char.hh \
  $(AUTOFILES)

# Char.hh should not be distributed but omitting it makes
# distcheck fail.

DISTCLEANFILES = $(AUTOFILES)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  Char.hh

noinst_HEADERS = \
  token.dec \
  token.def \
  entitiesTable.inc

SCRIPTDIR = ../scripts
CONFIGDIR = ../config

if COND_XSLTPROC
XSLTPROC = xsltproc

token.dec: token-list.xml list-to-dec.xsl
	$(XSLTPROC) $(srcdir)/list-to-dec.xsl $< >$@

token.def: token-list.xml list-to-def.xsl
	$(XSLTPROC) $(srcdir)/list-to-def.xsl $< >$@
endif

update-entities: $(CONFIGDIR)/entities-table.xml dumpEntitiesTable
	./dumpEntitiesTable $< >$@

config.dirs:
	echo "#define PKGDATADIR \"$(pkgdatadir)\"" >$@
	echo "#define PKGSYSCONFDIR \"$(sysconfdir)/@PACKAGE@\"" >>$@

all: config.dirs

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/frontend/gmetadom \
  -I$(top_builddir)

