
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

gtkmathview-config: gtkmathview-config.in
	@echo "#!/bin/sh" >$@
	@echo >>$@
	@echo VERSION=\"$(VERSION)\" >>$@
	@echo extra_cflags=\"$(DOM_CFLAGS) $(T1_CFLAGS) $(GTK_CFLAGS)\" >>$@
	@echo extra_libs=\"$(DOM_LIBS) $(T1_LIBS) $(GTK_LIBS)\" >>$@
	@echo prefix=\"$(prefix)\" >>$@
	@echo exec_prefix=\"$(exec_prefix)\" >>$@
	@echo libdir=\"$(libdir)\" >>$@
	@echo pkglibdir=\"$(pkglibdir)\" >>$@
	@echo includedir=\"$(includedir)\" >>$@
	@echo pkgincludedir=\"$(pkgincludedir)\" >>$@
	@echo domimplementation=\"$(DOM_IMPLEMENTATION)\" >>$@
	@echo >>$@
	@cat $< >>$@
	@chmod a+x gtkmathview-config

