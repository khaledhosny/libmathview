
bin_SCRIPTS = gtkmathview-config

noinst_SCRIPTS = \
  applylicence.pl \
  cutlicence.pl \
  mklicence.pl \
  stretchy \
  makekeywords \
  applylicence \
  makeEntitiesTable

EXTRA_DIST = gtkmathview-config.in $(noinst_SCRIPTS)

CLEANFILES = gtkmathview-config

gtkmathview-config:
	@echo "#!/bin/sh" >gtkmathview-config
	@echo >>gtkmathview-config
	@echo VERSION=\"$(VERSION)\" >>gtkmathview-config
	@echo libdir=\"$(libdir)\" >>gtkmathview-config
	@echo pkglibdir=\"$(pkglibdir)\" >>gtkmathview-config
	@echo includedir=\"$(includedir)\" >>gtkmathview-config
	@echo pkgincludedir=\"$(pkgincludedir)\" >>gtkmathview-config
	@echo >>gtkmathview-config
	@cat $(srcdir)/gtkmathview-config.in >>gtkmathview-config
	@chmod a+x gtkmathview-config
