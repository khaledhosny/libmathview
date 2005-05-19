
NULL =

AUTO_CC = \
  cmr10.cc \
  cmb10.cc \
  cmbxti10.cc \
  cmti10.cc \
  cmss10.cc \
  cmssi10.cc \
  cmssbx10.cc \
  cmtt10.cc \
  cmsy10.cc \
  cmbsy10.cc \
  cmmi10.cc \
  cmmib10.cc \
  cmex10.cc \
  $(NULL)

CLEANFILES = $(AUTO_CC)

noinst_LTLIBRARIES = libtfm.la

libtfm_la_SOURCES = \
  $(AUTO_CC) \
  TFM.hh \
  TFM.cc \
  TFMManager.hh \
  TFMManager.cc \
  $(NULL)

BUILT_SOURCES = $(AUTO_CC)

TFMDUMP = ~/Luca-CVS/Projects/xtfm/tfmdump

XTFMS = $(AUTO_CC:%.cc=$(srcdir)/%.xtfm)

$(srcdir)/%.xtfm: /usr/share/texmf/fonts/tfm/public/cm/%.tfm
	$(TFMDUMP) $< >$@

if COND_XSLTPROC
XSLTPROC = xsltproc
XTFMTOCC = $(top_srcdir)/auto/xtfm-to-cc.xsl
LICENSE = $(top_srcdir)/LICENSE
%.cc: $(srcdir)/%.xtfm $(XTFMTOCC) $(LICENSE)
	cat $(LICENSE) >$@
	$(XSLTPROC) --stringparam name $(@:%.cc=%) $(XTFMTOCC) $< >>$@

xtfms: $(XTFMS)

endif

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  $(NULL)
