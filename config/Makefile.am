
MAINTAINERCLEANFILES = \
  dictionary.xml \
  entities-table.xml \
  math-engine-configuration.xml

EXTRA_DIST = \
  dictionary.in \
  dictionary.dtd \
  dictionary.xml \
  dictionary-local.xml \
  dictionary-combining.xml \
  entities-table.in \
  entities-table.dtd \
  entities-table.xml \
  font-configuration.dtd \
  font-configuration.xml \
  font-configuration-adobe.xml \
  font-configuration-bh.xml \
  font-configuration-tex.xml \
  font-configuration-wri.xml \
  font-configuration-euclid.xml \
  font-configuration-mt.xml \
  math-engine-configuration.xml.in \
  math-engine-configuration.dtd \
  math-engine-configuration.xml \
  math-engine-configuration.xml.debug \
  t1.config

pkgdata_DATA = \
  dictionary.dtd \
  dictionary.xml \
  dictionary-local.xml \
  dictionary-combining.xml \
  entities-table.dtd \
  entities-table.xml \
  font-configuration.dtd \
  font-configuration.xml \
  font-configuration-adobe.xml \
  font-configuration-bh.xml \
  font-configuration-tex.xml \
  font-configuration-wri.xml \
  font-configuration-euclid.xml \
  font-configuration-mt.xml \
  math-engine-configuration.dtd \
  math-engine-configuration.xml \
  math-engine-configuration.xml.debug \
  t1.config

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
