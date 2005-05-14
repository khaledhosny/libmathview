
NULL =

if COND_SVG
MAYBE_SVG = mathmlsvg
else
MAYBE_SVG = $(NULL)
endif

EXTRA_DIST = BUGS HISTORY LICENSE ANNOUNCEMENT CONTRIBUTORS config.h.in
SUBDIRS = scripts config auto src doc viewer $(MAYBE_SVG)
CLEANFILES = core *.log *.eps

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = $(NULL)
if COND_LIBXML2
pkgconfig_DATA += mathview-libxml2.pc gtkmathview-libxml2.pc
endif
if COND_LIBXML2_READER
pkgconfig_DATA += gtkmathview-libxml2-reader.pc
endif
if COND_CUSTOM_READER
pkgconfig_DATA += gtkmathview-custom-reader.pc
endif
if COND_GMETADOM
pkgconfig_DATA += gtkmathview-gmetadom.pc
endif

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

