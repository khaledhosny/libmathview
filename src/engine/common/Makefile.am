
lib_LTLIBRARIES = libengine_common.la

libengine_common_la_LIBADD = \
  -lstdc++ \
  $(DOM_LIBS)

libengine_common_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libengine_common_la_SOURCES = \
  Attribute.cc \
  AttributeList.cc \
  AttributeSignature.cc \
  RefinementContext.cc

noinst_HEADERS = \
  Attribute.hh \
  AttributeList.hh \
  AttributeSignature.hh \
  AbstractRefinementContext.hh \
  RefinementContext.hh \
  BinContainerTemplate.hh \
  LinearContainerTemplate.hh \
  View.hh \
  ViewContext.hh \
  Element.hh 

INCLUDES = \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/src/common
