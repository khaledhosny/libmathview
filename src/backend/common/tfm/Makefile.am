
NULL =

TFM_LEVEL_1_CC = \
  cmex10.cc \
  cmmi10.cc \
  cmr10.cc \
  cmsy10.cc \
  $(NULL)

TFM_LEVEL_2_CC = \
  $(TFM_LEVEL_1_CC) \
  cmb10.cc \
  cmbsy10.cc \
  cmbxti10.cc \
  cmmib10.cc \
  cmss10.cc \
  cmssbx10.cc \
  cmssi10.cc \
  cmti10.cc \
  cmtt10.cc \
  msam10.cc \
  msbm10.cc \
  $(NULL)

TFM_LEVEL_3_CC = \
  $(TFM_LEVEL_2_CC) \
  cmbx10.cc \
  cmbx12.cc \
  cmbx5.cc \
  cmbx6.cc \
  cmbx7.cc \
  cmbx8.cc \
  cmbx9.cc \
  cmex9.cc \
  cmmi12.cc \
  cmmi5.cc \
  cmmi6.cc \
  cmmi7.cc \
  cmmi8.cc \
  cmmi9.cc \
  cmr12.cc \
  cmr17.cc \
  cmr5.cc \
  cmr6.cc \
  cmr7.cc \
  cmr8.cc \
  cmr9.cc \
  cmss12.cc \
  cmss17.cc \
  cmss8.cc \
  cmss9.cc \
  cmssi12.cc \
  cmssi17.cc \
  cmssi8.cc \
  cmssi9.cc \
  cmsy5.cc \
  cmsy6.cc \
  cmsy7.cc \
  cmsy8.cc \
  cmsy9.cc \
  cmti12.cc \
  cmti7.cc \
  cmti8.cc \
  cmti9.cc \
  cmtt12.cc \
  cmtt8.cc \
  cmtt9.cc \
  msam5.cc \
  msam6.cc \
  msam7.cc \
  msam8.cc \
  msam9.cc \
  msbm5.cc \
  msbm6.cc \
  msbm7.cc \
  msbm8.cc \
  msbm9.cc \
  $(NULL)

if COND_TFM_LEVEL_1
AUTO_CC = $(TFM_LEVEL_1_CC)
else
if COND_TFM_LEVEL_2
AUTO_CC = $(TFM_LEVEL_2_CC)
else
if COND_TFM_LEVEL_3
AUTO_CC = $(TFM_LEVEL_3_CC)
else
AUTO_CC = $(NULL)
endif
endif
endif

MAINTAINERCLEANFILES = $(AUTO_CC)

noinst_LTLIBRARIES = libtfm.la

BUILT_SOURCES = $(AUTO_CC)

libtfm_la_CPPFLAGS = -DGMV_MathView_DLL
libtfm_la_SOURCES = \
  $(AUTO_CC) \
  TFM.hh \
  TFM.cc \
  TFMManager.hh \
  TFMManager.cc \
  $(NULL)

TFMDUMP = ~/Luca-CVS/Projects/xtfm/tfmdump

XTFMS = $(TFM_LEVEL_3_CC:%.cc=$(srcdir)/%.xtfm)

# Linux cm* fonts
$(srcdir)/%.xtfm: /usr/share/texmf/fonts/tfm/public/cm/%.tfm
	$(TFMDUMP) $< >$@

# OSX cm* fonts
$(srcdir)/%.xtfm: /usr/local/teTeX/share/texmf.tetex/fonts/tfm/public/cm/%.tfm
	$(TFMDUMP) $< >$@

# OSX ms* fonts
$(srcdir)/%.xtfm: /usr/local/teTeX/share/texmf.tetex/fonts/tfm/ams/symbols/%.tfm
	$(TFMDUMP) $< >$@

if COND_XSLTPROC
XSLTPROC = xsltproc
XTFMTOCC = $(top_srcdir)/auto/xtfm-to-cc.xsl
LICENSE = $(top_srcdir)/doc/LICENSE.cc
%.cc: $(srcdir)/%.xtfm $(XTFMTOCC) $(LICENSE)
	cat $(LICENSE) >$@
	$(XSLTPROC) --stringparam name $(basename $(notdir $(@:%.cc=%))) $(XTFMTOCC) $< >>$@

.PHONY: xtfms
xtfms: $(XTFMS)

endif

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  $(NULL)
