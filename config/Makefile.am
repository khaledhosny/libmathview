
NULL =

MAINTERNERCLEANFILES = \
  gtkmathview.conf.xml \
  $(NULL)

EXTRA_DIST = \
  dictionary.dtd \
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
  gtkmathview.conf.dtd \
  $(NULL)

-include $(top_srcdir)/git.mk
