
noinst_PROGRAMS = dumpEntitiesTable

dumpEntitiesTable_SOURCES = dumpEntitiesTable.cc

AUTOFILES = \
  attribute.dec attribute.def \
  keyword.dec keyword.def \
  tag.dec tag.def \
  entitiesTable.inc

EXTRA_DIST = \
  config.dirs.in \
  config.dirs \
  keyword.list \
  stretchy.list \
  entitiesTable.inc \
  $(AUTOFILES)

MAINTAINERCLEANFILES = $(AUTOFILES)

noinst_HEADERS = $(AUTOFILES)

SCRIPTDIR = ../scripts
CONFIGDIR = ../config

keyword.dec: keyword.list $(SCRIPTDIR)/makekeywords
	@$(SCRIPTDIR)/makekeywords dec Keyword KW $@

keyword.def: keyword.list $(SCRIPTDIR)/makekeywords
	@$(SCRIPTDIR)/makekeywords def Keyword KW $@

tag.dec: keyword.list $(SCRIPTDIR)/makekeywords
	@$(SCRIPTDIR)/makekeywords dec Tag TAG $@

tag.def: keyword.list $(SCRIPTDIR)/makekeywords
	@$(SCRIPTDIR)/makekeywords def Tag TAG $@

attribute.dec: keyword.list $(SCRIPTDIR)/makekeywords
	@$(SCRIPTDIR)/makekeywords dec Attribute ATTR $@

attribute.def: keyword.list $(SCRIPTDIR)/makekeywords
	@$(SCRIPTDIR)/makekeywords def Attribute ATTR $@

entitiesTable.inc: $(CONFIGDIR)/entities-table.xml dumpEntitiesTable
	./dumpEntitiesTable $< >$@