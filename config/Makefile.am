
MAINTAINERCLEANFILES = \
  dictionary.xml \
  entities-table.xml \
  math-engine-configuration.xml

EXTRA_DIST = \
  dictionary.in \
  dictionary.dtd \
  dictionary.xml \
  entities-table.in \
  entities-table.dtd \
  entities-table.xml \
  font-configuration.dtd \
  font-configuration.xml \
  math-engine-configuration.xml.in \
  math-engine-configuration.dtd \
  math-engine-configuration.xml \
  t1.config

pkgdata_DATA = \
  dictionary.dtd \
  dictionary.xml \
  entities-table.dtd \
  entities-table.xml \
  font-configuration.dtd \
  font-configuration.xml \
  math-engine-configuration.dtd \
  math-engine-configuration.xml \
  t1.config

noinst_PROGRAMS = makeDictionary
makeDictionary_SOURCES = makeDictionary.c

SCRIPTDIR = $(srcdir)/scripts

dictionary.xml: makeDictionary.c dictionary.in
	@echo Making operator dictionary...
	@( \
	./makeDictionary <dictionary.in | \
	tr -s '\t' ' ' | tr -s ' ' ' ' >dictionary.xml \
	)

entities-table.xml: entities-table.in
	@echo Making entities table...
	$(SCRIPTDIR)/makeEntitiesTable <entities-table.in >entities-table.xml

#math-engine-configuration.xml: math-engine-configuration.in
#	@echo Making configuration file...
