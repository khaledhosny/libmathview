EXTRA_DIST = BUGS HISTORY LICENSE ANNOUNCEMENT CONTRIBUTORS config.h.in
SUBDIRS = scripts config auto src doc viewer mathml2ps randomath
CLEANFILES = core *.log *.eps

SCRIPTDIR = ./scripts

backup:
	tar cvfz ../@PACKAGE@-@VERSION@-`date|tr ' ' '_'|tr ':' '_'`.tar.gz .

cleanbak:
	-rm -f `find . -name "*~"`

licence:
	$(SCRIPTDIR)/mklicence.pl c <doc/LICENCE >doc/LICENCEc
	$(SCRIPTDIR)/mklicence.pl cc <doc/LICENCE >doc/LICENCEcc
	$(SCRIPTDIR)/applylicence $(SCRIPTDIR)

lc:
	@( \
	CFILES=`find . -name "*.c"`; \
	HFILES=`find . -name "*.h"`; \
	CCFILES=`find . -name "*.cc"`; \
	HHFILES=`find . -name "*.hh"`; \
	ICCFILES=`find . -name "*.icc"`; \
	wc -l $$CFILES $$HFILES $$CCFILES $$HHFILES $$ICCFILES | tail -n 1 \
	)

profile:
