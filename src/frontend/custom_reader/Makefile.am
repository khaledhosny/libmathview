
NULL = 

lib_LTLIBRARIES = libmathview_frontend_custom_reader.la

libmathview_frontend_custom_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_frontend_custom_reader_la_LIBADD = \
  $(top_builddir)/src/frontend/common/libfrontend_common.la \
  $(top_builddir)/src/libmathview.la

libmathview_frontend_custom_reader_la_SOURCES = \
  customXmlReader.cc \
  custom_reader_Builder.cc

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  c_customXmlReader.h \
  $(NULL)

noinst_HEADERS = \
  customXmlReader.hh \
  custom_reader_Model.hh \
  custom_reader_Builder.hh \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/engine/adapters \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/formatter/areamodel 

