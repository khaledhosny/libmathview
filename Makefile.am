ACLOCAL_AMFLAGS = -I m4

NULL =

EXTRA_DIST = BUGS HISTORY LICENSE ANNOUNCEMENT CONTRIBUTORS config.h.in
SUBDIRS = scripts config auto src doc viewer

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = mathview-core.pc
if HAVE_LIBXML2
pkgconfig_DATA += mathview-frontend-libxml2.pc
endif
if HAVE_LIBXML2_READER
pkgconfig_DATA += mathview-frontend-libxml2-reader.pc
endif
if HAVE_CUSTOM_READER
pkgconfig_DATA += mathview-frontend-custom-reader.pc
endif
if HAVE_GMETADOM
pkgconfig_DATA += mathview-frontend-gmetadom.pc
endif

if HAVE_CAIRO
pkgconfig_DATA += mathview-backend-cairo.pc
endif
if HAVE_QT
pkgconfig_DATA += mathview-backend-qt.pc
endif
if HAVE_LIBXML2
pkgconfig_DATA += gtkmathview-libxml2.pc
endif
if HAVE_LIBXML2_READER
pkgconfig_DATA += gtkmathview-libxml2-reader.pc
endif
if HAVE_CUSTOM_READER
pkgconfig_DATA += gtkmathview-custom-reader.pc
endif
if HAVE_GMETADOM
pkgconfig_DATA += gtkmathview-gmetadom.pc
endif

SCRIPTDIR = ./scripts

backup:
	tar cvfz ../@PACKAGE@-@PACKAGE_VERSION@-`date|tr ' ' '_'|tr ':' '_'`.tar.gz .

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
	diff -r src/ @PACKAGE@-@PACKAGE_VERSION@/src/ | grep '.\(cc\|hh\)$$'

MAINTAINERCLEANFILES = \
		       $(GITIGNORE_MAINTAINERCLEANFILES_TOPLEVEL) \
		       $(GITIGNORE_MAINTAINERCLEANFILES_MAKEFILE_IN) \
		       $(GITIGNORE_MAINTAINERCLEANFILES_M4_LIBTOOL)

-include $(top_srcdir)/git.mk
