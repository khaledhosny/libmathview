
NULL =

EXTRA_DIST = BUGS HISTORY LICENSE ANNOUNCEMENT CONTRIBUTORS config.h.in
SUBDIRS = scripts config auto src doc viewer
CLEANFILES = core *.log *.eps

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = \
  mathview-libxml2.pc \
  gtkmathview-custom-reader.pc \
  gtkmathview-libxml2-reader.pc \
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
	CFILES=`find $(top_srcdir) -name "*.c"`; \
	HFILES=`find $(top_srcdir) -name "*.h"`; \
	CCFILES=`find $(top_srcdir) -name "*.cc"`; \
	HHFILES=`find $(top_srcdir) -name "*.hh"`; \
	ICCFILES=`find $(top_srcdir) -name "*.icc"`; \
	wc -l $$CFILES $$HFILES $$CCFILES $$HHFILES $$ICCFILES | tail -n 1 \
	)

distdiff:
	diff -r src/ @PACKAGE@-@VERSION@/src/ | grep '.\(cc\|hh\)$$'

