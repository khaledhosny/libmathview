
if COND_TFM
MAYBE_TFM = tfm
MAYBE_LIBTFM = $(top_builddir)/src/formatter/common/tfm/libtfm.la
else
MAYBE_TFM =
MAYBE_LIBTFM =
endif

SUBDIRS = $(MAYBE_TFM)

NULL =

if COND_T1LIB

if COND_TFM
MAYBE_T1_TFM_S = \
  t1lib_TFM_T1Font.hh \
  t1lib_TFM_T1Font.cc \
  t1lib_TFM_T1FontManager.hh \
  t1lib_TFM_T1FontManager.cc \
  $(NULL)
else
MAYBE_T1_TFM_S = $(NULL)
endif

MAYBE_T1_S = \
  t1lib_T1Font.cc \
  t1lib_T1FontManager.cc \
  t1lib_T1Font.hh \
  t1lib_T1FontManager.hh \
  $(MAYBE_T1_TFM_S) \
  $(NULL)
else
MAYBE_T1_S = $(NULL)
endif

if COND_TFM
MAYBE_TFM_S = \
  TFMFont.hh \
  TFMFont.cc \
  TFMFontManager.hh \
  TFMFontManager.cc \
  $(NULL)

if COND_SVG
MAYBE_SVG_S = \
  SVG_T1Font.hh \
  SVG_T1FontManager.hh \
  $(NULL)
else
MAYBE_SVG_S = $(NULL)
endif

else
MAYBE_TFM_S = $(NULL)
MAYBE_SVG_S = $(NULL)
endif

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  FormattingContext.hh \
  GraphicDevice.hh \
  T1Font.hh \
  $(NULL)

noinst_LTLIBRARIES = libcommon.la

libcommon_la_SOURCES = \
  FormattingContext.cc \
  GraphicDevice.cc \
  $(MAYBE_TFM_S) \
  $(MAYBE_T1_S) \
  $(MAYBE_SVG_S) \
  $(NULL)

libcommon_la_LIBADD = \
  $(MAYBE_LIBTFM) \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/formatter/common \
  -I$(top_srcdir)/src/formatter/common/tfm \
  -I$(top_srcdir)/src/formatter/areamodel \
  -I$(top_srcdir)/src/formatter/mathml_graphic_device \
  -I$(top_srcdir)/src/formatter/boxml_graphic_device \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  $(NULL)

