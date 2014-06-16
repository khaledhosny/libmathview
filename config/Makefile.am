
NULL =

MAINTERNERCLEANFILES = \
  gtkmathview.conf.xml \
  $(NULL)

EXTRA_DIST = \
  mathml-entities.dtd \
  dictionary.in \
  dictionary.dtd \
  dictionary.xml \
  dictionary-local.xml \
  dictionary-combining.xml \
  gtkmathview.conf.xml.in \
  gtkmathview.conf.dtd \
  gtkmathview.conf.xml \
  Makefile.slow \
  $(NULL)

pkgsysconfdir = $(sysconfdir)/@PACKAGE@
pkgsysconf_DATA = \
  gtkmathview.conf.xml

pkgdata_DATA = \
  mathml-entities.dtd \
  dictionary.dtd \
  dictionary.xml \
  dictionary-local.xml \
  dictionary-combining.xml \
  gtkmathview.conf.dtd \
  $(NULL)

-include $(top_srcdir)/git.mk
