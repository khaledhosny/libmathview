
NULL =

AUTOFILES = \
  token.dec \
  token.def \
  config.dirs \
  $(NULL)

EXTRA_DIST = \
  list-to-dec.xsl \
  list-to-def.xsl \
  token-list.xml \
  stretchy.list \
  Char.hh.in \
  token.dec \
  token.def \
  entitiesTable.inc \
  $(NULL)

DISTCLEANFILES = config.dirs Char.hh
MAINTAINERCLEANFILES = token.dec token.def

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  Char.hh

noinst_HEADERS = $(AUTOFILES)

SCRIPTDIR = ../scripts
CONFIGDIR = ../config

token.dec: token-list.xml list-to-dec.xsl
	$(AM_V_GEN)$(XSLTPROC) $(srcdir)/list-to-dec.xsl $< >$@

token.def: token-list.xml list-to-def.xsl
	$(AM_V_GEN)$(XSLTPROC) $(srcdir)/list-to-def.xsl $< >$@

.PHONY: config.dirs
config.dirs:
	$(AM_V_GEN)( \
	echo "#define PKGDATADIR \"$(pkgdatadir)\"" >$@.maybe; \
	echo "#define PKGSYSCONFDIR \"$(sysconfdir)/@PACKAGE@\"" >>$@.maybe; \
	if test ! \( -e $@ \); then \
	  mv $@.maybe $@; \
	elif ! diff $@.maybe $@ >/dev/null; then \
	  mv $@.maybe $@; \
	else \
	  rm $@.maybe; \
	fi \
	)

-include $(top_srcdir)/git.mk
