
SUBDIRS = boxml mathml

lib_LTLIBRARIES = libengine.la

libengine_la_LIBADD = \
  -lstdc++ \
  $(DOM_LIBS)

libengine_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libengine_la_SOURCES = \
  AttributeSignature.cc

noinst_HEADERS = \
  BinContainerTemplate.hh \
  LinearContainerTemplate.hh \
  View.hh \
  ViewContext.hh \
  Element.hh 

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/formatter/basictypes