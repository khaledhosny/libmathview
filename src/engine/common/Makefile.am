
NULL =

noinst_LTLIBRARIES = libengine_common.la

libengine_common_la_CPPFLAGS = -DGMV_MathView_DLL
libengine_common_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libengine_common_la_SOURCES = \
  Attribute.cc \
  AttributeSet.cc \
  AttributeSignature.cc \
  NamespaceContext.cc \
  View.cc \
  Node.cc \
  Element.cc \
  prefix.c \
  prefix.h \
  $(NULL)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = \
  NamespaceContext.hh \
  Element.hh \
  Node.hh \
  View.hh \
  $(NULL)

noinst_HEADERS = \
  Adapters.hh \
  Attribute.hh \
  AttributeSet.hh \
  AttributeSignature.hh \
  BinContainerTemplate.hh \
  CommonStringParsers.hh \
  ElementFactory.hh \
  LinearContainerTemplate.hh \
  $(NULL)

INCLUDES = \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/engine/boxml \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/backend/common \
  $(NULL)

