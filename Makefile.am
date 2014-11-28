ACLOCAL_AMFLAGS = -I m4

NULL =

EXTRA_DIST = BUGS HISTORY LICENSE CONTRIBUTORS config.h.in
SUBDIRS = scripts auto src doc viewer

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

if HAVE_CAIRO
pkgconfig_DATA += mathview-backend-cairo.pc
endif
if HAVE_QT
pkgconfig_DATA += mathview-backend-qt.pc
endif
if HAVE_LIBXML2
pkgconfig_DATA += gtkmathview-libxml2.pc
if HAVE_QT
pkgconfig_DATA += mathview-widget-qt.pc
endif
endif
if HAVE_LIBXML2_READER
pkgconfig_DATA += gtkmathview-libxml2-reader.pc
endif
if HAVE_CUSTOM_READER
pkgconfig_DATA += gtkmathview-custom-reader.pc
endif

MAINTAINERCLEANFILES = \
		       $(GITIGNORE_MAINTAINERCLEANFILES_TOPLEVEL) \
		       $(GITIGNORE_MAINTAINERCLEANFILES_MAKEFILE_IN) \
		       $(GITIGNORE_MAINTAINERCLEANFILES_M4_LIBTOOL)

GITIGNOREFILES = .DS_Store

-include $(top_srcdir)/git.mk
