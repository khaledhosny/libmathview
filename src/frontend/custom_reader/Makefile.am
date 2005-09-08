
NULL = 

noinst_LTLIBRARIES = libfrontend_custom_reader.la

libfrontend_custom_reader_la_CPPFLAGS = -DGMV_FrontEnd_DLL
libfrontend_custom_reader_la_LIBADD = $(NULL)

libfrontend_custom_reader_la_SOURCES = \
  customXmlReader.cc \
  custom_reader_Builder.cc \
  $(NULL)

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
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/engine/adapters \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/backend/common \
  $(NULL)

