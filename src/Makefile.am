
SUBDIRS = formatter engine

GTK_CFLAGS = @GTK_CFLAGS@
GTK_LIBS = @GTK_LIBS@

DOM_CFLAGS = @DOM_CFLAGS@
DOM_LIBS = @DOM_LIBS@

T1_CFLAGS = @T1_CFLAGS@
T1_LIBS = @T1_LIBS@

lib_LTLIBRARIES = libgtkmathview.la

libgtkmathview_la_LIBADD = \
  -lstdc++ \
  $(GTK_LIBS) \
  $(DOM_LIBS) \
  $(top_builddir)/src/engine/libengine.la \
  $(top_builddir)/src/formatter/gtk_mathml_graphic_device/libgtk_mathml_graphic_device.la

libgtkmathview_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

GTKSOURCES = \
  Gtk_DrawingArea.cc \
  Gtk_GraphicsContext.cc

GTKHEADERS = \
  Gtk_DrawingArea.hh \
  Gtk_GraphicsContext.hh

GUISOURCES = \
  DrawingArea.cc \
  GraphicsContext.cc \
  gtkmathview.cc

libgtkmathview_la_SOURCES = \
  Clock.cc \
  Configuration.cc \
  EntitiesTable.cc \
  Logger.cc \
  Globals.cc \
  ChildList.cc \
  MathMLDOMLinker.cc \
  OperatorDictionary.cc \
  Rectangle.cc \
  ValueConversion.cc \
  frameAux.cc \
  gmetadom.cc \
  scaledConv.cc \
  token.cc \
  operatorAux.cc \
  traverseAux.cc \
  mathVariantAux.cc \
  $(GUISOURCES) \
  $(GTKSOURCES)

pkginclude_HEADERS = \
  defs.h \
  gtkmathview.h

noinst_HEADERS = \
  Adaptors.hh \
  Clock.hh \
  Configuration.hh \
  Coords.hh \
  DrawingArea.hh \
  EntitiesTable.hh \
  GraphicsContext.hh \
  Logger.hh \
  Globals.hh \
  MathML.hh \
  ChildList.hh \
  FormattingContext.hh \
  MathMLDOMLinker.hh \
  OperatorDictionary.hh \
  Rectangle.hh \
  ValueConversion.hh \
  frameAux.hh \
  gmetadom.hh \
  token.hh \
  operatorAux.hh \
  traverseAux.hh \
  mathVariantAux.hh \
  scaledConv.hh \
  for_each_if.h \
  unidefs.h \
  $(GTKHEADERS)

INCLUDES = \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  -I$(top_builddir) \
  -I$(top_srcdir)/auto/ \
  -I$(srcdir)/formatter/basictypes \
  -I$(srcdir)/formatter/basictypes/mathvariants/ \
  -I$(srcdir)/formatter/areamodel \
  -I$(srcdir)/formatter/gtk_areamodel \
  -I$(srcdir)/formatter/mathml_graphic_device \
  -I$(srcdir)/formatter/gtk_mathml_graphic_device \
  -I$(srcdir)/engine \
  -I$(srcdir)/engine/mathml


