
NULL =

EXTRA_DIST = BUGS HISTORY LICENSE ANNOUNCEMENT CONTRIBUTORS config.h.in
SUBDIRS = scripts config auto src doc viewer
CLEANFILES = core *.log *.eps

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = \
  mathview-libxml2.pc \
  gtkmathview-custom.pc \
  gtkmathview-libxml2.pc \
  gtkmathview-gmetadom.pc \
  $(NULL)

SCRIPTDIR = ./scripts

backup:
	tar cvfz ../@PACKAGE@-@VERSION@-`date|tr ' ' '_'|tr ':' '_'`.tar.gz .

cleanbak:
	-rm -f `find . -name "*~"`

lc:
	@( \
	CFILES=`find . -name "*.c"`; \
	HFILES=`find . -name "*.h"`; \
	CCFILES=`find . -name "*.cc"`; \
	HHFILES=`find . -name "*.hh"`; \
	ICCFILES=`find . -name "*.icc"`; \
	wc -l $$CFILES $$HFILES $$CCFILES $$HHFILES $$ICCFILES | tail -n 1 \
	)

distdiff:
	diff -r src/ @PACKAGE@-@VERSION@/src/ | grep '.\(cc\|hh\)$$'

