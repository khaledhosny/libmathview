
NULL =

MAINTERNERCLEANFILES = \
  gtkmathview.conf.xml \
  $(NULL)

EXTRA_DIST = \
  dictionary.dtd \
  dictionary-local.xml \
  gtkmathview.conf.xml.in \
  gtkmathview.conf.dtd \
  gtkmathview.conf.xml \
  Makefile.slow \
  $(NULL)

pkgsysconfdir = $(sysconfdir)/@PACKAGE@
pkgsysconf_DATA = \
  gtkmathview.conf.xml

pkgdata_DATA = \
  dictionary.dtd \
  dictionary-local.xml \
  gtkmathview.conf.dtd \
  $(NULL)

-include $(top_srcdir)/git.mk
