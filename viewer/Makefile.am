
DOM_CFLAGS = @DOM_CFLAGS@
DOM_LIBS = @DOM_LIBS@

#bin_PROGRAMS = mathmlviewer
noinst_PROGRAMS = mathmlviewer.debug

noinst_HEADERS = guiGTK.h

#mathmlviewer_SOURCES = main.cc aux.cc guiGTK.c

#mathmlviewer_LDADD = \
  #$(DOM_LIBS) \
  #$(GTK_LIBS) \
  #$(top_builddir)/src/libgtkmathview.la

mathmlviewer_debug_SOURCES = main.cc aux.cc guiGTK.c

mathmlviewer_debug_LDADD = \
  $(DOM_LIBS) \
  $(GTK_LIBS) \
  $(top_builddir)/src/widget/libwidget.la \
  $(top_builddir)/src/engine/mathml/libmathml.la \
  $(top_builddir)/src/engine/boxml/libboxml.la \
  $(top_builddir)/src/formatter/gtk_mathml_graphic_device/libgtk_mathml_graphic_device.la \
  $(top_builddir)/src/formatter/mathml_graphic_device/libmathml_graphic_device.la \
  $(top_builddir)/src/formatter/basictypes/mathvariants/libmathvariants.la \
  $(top_builddir)/src/engine/common/libengine_common.la \
  $(top_builddir)/src/common/libcommon.la

INCLUDES = \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  -I$(top_srcdir)/auto/ \
  -I$(top_srcdir)/src/common/ \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/widget \
  -I$(top_srcdir)/src/formatter/areamodel/

