
NULL =

if COND_GTK
if COND_HAVE_POPT
MAYBE_GTK_SUBDIRS = viewer
else
MAYBE_GTK_SUBDIRS = $(NULL)
endif
else
MAYBE_GTK_SUBDIRS = $(NULL)
endif

if COND_SVG
if COND_HAVE_POPT
MAYBE_SVG_SUBDIRS = mathmlsvg
else
MAYBE_SVG_SUBDIRS = $(NULL)
endif
else
MAYBE_SVG_SUBDIRS = $(NULL)
endif

if COND_PS
if COND_HAVE_POPT
MAYBE_PS_SUBDIRS = mathmlps
else
MAYBE_PS_SUBDIRS = $(NULL)
endif
else
MAYBE_PS_SUBDIRS = $(NULL)
endif

EXTRA_DIST = BUGS HISTORY LICENSE ANNOUNCEMENT CONTRIBUTORS config.h.in README.MacOSX
SUBDIRS = scripts config auto autopackage src doc $(MAYBE_GTK_SUBDIRS) $(MAYBE_SVG_SUBDIRS) $(MAYBE_PS_SUBDIRS)
CLEANFILES = core *.log *.eps

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = mathview-core.pc
if COND_LIBXML2
pkgconfig_DATA += mathview-frontend-libxml2.pc
endif
if COND_LIBXML2_READER
pkgconfig_DATA += mathview-frontend-libxml2-reader.pc
endif
if COND_CUSTOM_READER
pkgconfig_DATA += mathview-frontend-custom-reader.pc
endif
if COND_GMETADOM
pkgconfig_DATA += mathview-frontend-gmetadom.pc
endif

if COND_GTK
pkgconfig_DATA += mathview-backend-gtk.pc
if COND_LIBXML2
pkgconfig_DATA += gtkmathview-libxml2.pc
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
endif

if COND_SVG
pkgconfig_DATA += mathview-backend-svg.pc
endif

if COND_PS
pkgconfig_DATA += mathview-backend-ps.pc
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

