
NULL =

noinst_LTLIBRARIES = libfrontend_common.la

libfrontend_common_la_CPPFLAGS = -DGMV_MathView_DLL
libfrontend_common_la_SOURCES = \
  Builder.cc \
  $(NULL)

noinst_HEADERS = \
  Builder.hh \
  TemplateBuilder.hh \
  TemplateLinker.hh \
  TemplateRefinementContext.hh \
  TemplateReaderModel.hh \
  TemplateReaderBuilder.hh \
  TemplateReaderNodeIterator.hh \
  TemplateReaderElementIterator.hh \
  TemplateReaderRefinementContext.hh \
  TemplateSetup.hh \
  Reader.hh \
  $(NULL)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  TemplateNodeIterator.hh \
  TemplateElementValidator.hh \
  TemplateElementIterator.hh \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/backend/common \
  $(NULL)

