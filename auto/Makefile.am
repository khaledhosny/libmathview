
AUTOFILES = \
  attribute.dec attribute.def \
  keyword.dec keyword.def \
  tag.dec tag.def

EXTRA_DIST = keyword.list stretchy.list entitiesTable.inc $(AUTOFILES)

MAINTAINERCLEANFILES = $(AUTOFILES)

noinst_HEADERS = $(AUTOFILES)

SCRIPTDIR = ../scripts

keyword.dec: keyword.list
	@$(SCRIPTDIR)/makekeywords dec Keyword KW keyword.dec

keyword.def: keyword.list
	@$(SCRIPTDIR)/makekeywords def Keyword KW keyword.def

tag.dec: keyword.list
	@$(SCRIPTDIR)/makekeywords dec Tag TAG tag.dec

tag.def: keyword.list
	@$(SCRIPTDIR)/makekeywords def Tag TAG tag.def

attribute.dec: keyword.list
	@$(SCRIPTDIR)/makekeywords dec Attribute ATTR attribute.dec

attribute.def: keyword.list
	@$(SCRIPTDIR)/makekeywords def Attribute ATTR attribute.def

