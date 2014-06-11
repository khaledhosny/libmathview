
NULL =

AUTOFILES = \
  token.dec \
  token.def \
  $(NULL)

EXTRA_DIST = \
  list-to-dec.xsl \
  list-to-def.xsl \
  token-list.xml \
  stretchy.list \
  token.dec \
  token.def \
  entitiesTable.inc \
  $(NULL)

DISTCLEANFILES = $(NULL)
MAINTAINERCLEANFILES = token.dec token.def

mathviewdir = $(pkgincludedir)/MathView

noinst_HEADERS = $(AUTOFILES)

SCRIPTDIR = ../scripts
CONFIGDIR = ../config

token.dec: token-list.xml list-to-dec.xsl
	$(AM_V_GEN)$(XSLTPROC) $(srcdir)/list-to-dec.xsl $< >$@

token.def: token-list.xml list-to-def.xsl
	$(AM_V_GEN)$(XSLTPROC) $(srcdir)/list-to-def.xsl $< >$@

-include $(top_srcdir)/git.mk
