
NULL =

DISTCLEANFILES = \
  dictionary.xml \
  entities-table.xml \
  gtkmathview.conf.xml

EXTRA_DIST = \
  dictionary.in \
  dictionary.dtd \
  dictionary.xml \
  dictionary-local.xml \
  dictionary-combining.xml \
  entities-table.in \
  entities-table.dtd \
  entities-table.xml \
  gtkmathview.conf.xml.in \
  gtkmathview.conf.dtd \
  gtkmathview.conf.xml \
  gtkmathview.conf.xml.debug \
  $(NULL)

pkgdata_DATA = \
  dictionary.dtd \
  dictionary.xml \
  dictionary-local.xml \
  dictionary-combining.xml \
  entities-table.dtd \
  entities-table.xml \
  gtkmathview.conf.dtd \
  gtkmathview.conf.xml \
  $(NULL)

noinst_DATA = \
  gtkmathview.conf.xml.debug

noinst_PROGRAMS = makeDictionary
makeDictionary_SOURCES = makeDictionary.c

SCRIPTDIR = $(srcdir)/../scripts

dictionary.xml: makeDictionary dictionary.in
	@echo Making operator dictionary...
	@( \
	./makeDictionary <dictionary.in | \
	tr -s '\t' ' ' | tr -s ' ' ' ' >$@ \
	)

entities-table.xml: entities-table.in
	@echo Making entities table...
	$(SCRIPTDIR)/makeEntitiesTable <entities-table.in >$@
