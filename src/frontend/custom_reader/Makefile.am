
lib_LTLIBRARIES = libmathview_frontend_custom_reader.la

libmathview_frontend_custom_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_frontend_custom_reader_la_LIBADD = \
  $(top_builddir)/src/frontend/common/libfrontend_common.la

libmathview_frontend_custom_reader_la_SOURCES = \
  customXmlReader.cc \
  custom_reader_Builder.cc

noinst_HEADERS = \
  c_customXmlReader.h \
  customXmlReader.hh \
  custom_reader_Model.hh \
  custom_reader_Builder.hh

INCLUDES = \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/engine/adapters \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/formatter/areamodel 

