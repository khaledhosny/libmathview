
MAINTAINERCLEANFILES = dictionary.xml

EXTRA_DIST = \
  dictionary.dtd dictionary.in dictionary.xml \
  font-configuration.dtd font-configuration.xml

pkgdata_DATA = \
  dictionary.dtd dictionary.xml \
  font-configuration.dtd font-configuration.xml

noinst_PROGRAMS = makeDictionary
makeDictionary_SOURCES = makeDictionary.c

dictionary.xml: makeDictionary.c dictionary.in
	@echo Making operator dictionary...
	@( \
	./makeDictionary <dictionary.in | \
	tr -s '\t' ' ' | tr -s ' ' ' ' >dictionary.xml \
	)

