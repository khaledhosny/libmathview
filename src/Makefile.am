
NULL =

lib_LTLIBRARIES = $(NULL)
noinst_LTLIBRARIES = $(NULL)

mathviewdir = $(pkgincludedir)/MathView
mathview_HEADERS = $(NULL)
noinst_HEADERS = $(NULL)

EXTRA_DIST = $(NULL)
BUILT_SOURCES = $(NULL)
CLEANFILES = $(NULL)


lib_LTLIBRARIES += libmathview.la

libmathview_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined

libmathview_la_SOURCES = dummy.cc

libmathview_la_LIBADD = \
  libbackend.la \
  libfrontend_common.la \
  libengine.la \
  libcommon.la \
  $(NULL)



noinst_LTLIBRARIES += libcommon.la

libcommon_la_SOURCES = \
  common/AbstractLogger.cc \
  common/BoundingBox.cc \
  common/BoundingBoxAux.cc \
  common/Clock.cc \
  common/LengthAux.cc \
  common/Logger.cc \
  common/PointAux.cc \
  common/RGBColorAux.cc \
  common/Rectangle.cc \
  common/String.cc \
  common/StringAux.cc \
  common/StringHash.cc \
  common/ValueConversion.cc \
  common/token.cc \
  $(NULL)

mathview_HEADERS += \
  common/AbstractLogger.hh \
  common/BoundingBox.hh \
  common/BoundingBoxAux.hh \
  common/Length.hh \
  common/LengthAux.hh \
  common/Logger.hh \
  common/MathVariant.hh \
  common/Object.hh \
  common/Point.hh \
  common/PointAux.hh \
  common/RGBColor.hh \
  common/RGBColorAux.hh \
  common/Rectangle.hh \
  common/SmartPtr.hh \
  common/ScopedHashMap.hh \
  common/FastScopedHashMap.hh \
  common/SparseMap.hh \
  common/String.hh \
  common/StringAux.hh \
  common/StringHash.hh \
  common/Value.hh \
  common/ValueConversion.hh \
  common/Variant.hh \
  common/WeakPtr.hh \
  common/defs.h \
  common/fixed.hh \
  common/scaled.hh \
  common/scaledAux.hh \
  $(NULL)

noinst_HEADERS += \
  common/CharTraits.hh \
  common/Clock.hh \
  common/GObjectPtr.hh \
  common/TemplateStringScanners.hh \
  common/TemplateStringParsers.hh \
  common/TokenSet.hh \
  common/for_each_if.h \
  common/token.hh \
  $(NULL)



noinst_LTLIBRARIES += libengine.la

libengine_la_SOURCES = \
  engine/Attribute.cc \
  engine/AttributeSet.cc \
  engine/AttributeSignature.cc \
  engine/Element.cc \
  engine/MathMLActionElement.cc \
  engine/MathMLAlignGroupElement.cc \
  engine/MathMLAlignMarkElement.cc \
  engine/MathMLAttributeSignatures.cc \
  engine/MathMLBinContainerElement.cc \
  engine/MathMLContainerElement.cc \
  engine/MathMLDummyElement.cc \
  engine/MathMLElement.cc \
  engine/MathMLEmbellishment.cc \
  engine/MathMLEncloseElement.cc \
  engine/MathMLErrorElement.cc \
  engine/MathMLFractionElement.cc \
  engine/MathMLFunctionApplicationNode.cc \
  engine/MathMLGlyphNode.cc \
  engine/MathMLIdentifierElement.cc \
  engine/MathMLInferredRowElement.cc \
  engine/MathMLInvisibleTimesNode.cc \
  engine/MathMLLinearContainerElement.cc \
  engine/MathMLMarkNode.cc \
  engine/MathMLmathElement.cc \
  engine/MathMLMultiScriptsElement.cc \
  engine/MathMLNamespaceContext.cc \
  engine/MathMLNode.cc \
  engine/MathMLNormalizingContainerElement.cc \
  engine/MathMLNumberElement.cc \
  engine/MathMLOperatorDictionary.cc \
  engine/MathMLOperatorElement.cc \
  engine/MathMLPaddedElement.cc \
  engine/MathMLPhantomElement.cc \
  engine/MathMLRadicalElement.cc \
  engine/MathMLRowElement.cc \
  engine/MathMLScriptElement.cc \
  engine/MathMLSpaceElement.cc \
  engine/MathMLStringLitElement.cc \
  engine/MathMLStringNode.cc \
  engine/MathMLStyleElement.cc \
  engine/MathMLTableCellElement.cc \
  engine/MathMLTableContentFactory.cc \
  engine/MathMLTableElement.cc \
  engine/MathMLTableFormatter.cc \
  engine/MathMLTextElement.cc \
  engine/MathMLTextNode.cc \
  engine/MathMLTokenElement.cc \
  engine/MathMLUnderOverElement.cc \
  engine/MathMLValueConversion.cc \
  engine/mathVariantAux.cc \
  engine/NamespaceContext.cc \
  engine/Node.cc \
  engine/traverseAux.cc \
  engine/View.cc \
  $(NULL)

mathview_HEADERS += \
  engine/Element.hh \
  engine/MathMLElement.hh \
  engine/MathMLNamespaceContext.hh \
  engine/MathMLOperatorDictionary.hh \
  engine/NamespaceContext.hh \
  engine/Node.hh \
  engine/View.hh \
  $(NULL)

noinst_HEADERS += \
  engine/Adapters.hh \
  engine/Attribute.hh \
  engine/AttributeSet.hh \
  engine/AttributeSignature.hh \
  engine/BinContainerTemplate.hh \
  engine/CommonStringParsers.hh \
  engine/ElementFactory.hh \
  engine/LinearContainerTemplate.hh \
  engine/MathMLActionElement.hh \
  engine/MathMLAdapters.hh \
  engine/MathMLAlignGroupElement.hh \
  engine/MathMLAlignMarkElement.hh \
  engine/MathMLAttributeParsers.hh \
  engine/MathMLAttributeParsers.hh \
  engine/MathMLAttributeSignatures.hh \
  engine/MathMLBinContainerElement.hh \
  engine/MathMLContainerElement.hh \
  engine/MathMLDummyElement.hh \
  engine/MathMLEmbellishment.hh \
  engine/MathMLEncloseElement.hh \
  engine/MathMLErrorElement.hh \
  engine/MathMLFractionElement.hh \
  engine/MathMLFunctionApplicationNode.hh \
  engine/MathMLGlyphNode.hh \
  engine/MathML.hh \
  engine/MathMLIdentifierElement.hh \
  engine/MathMLInferredRowElement.hh \
  engine/MathMLInvisibleTimesNode.hh \
  engine/MathMLLinearContainerElement.hh \
  engine/MathMLMarkNode.hh \
  engine/MathMLmathElement.hh \
  engine/MathMLMultiScriptsElement.hh \
  engine/MathMLNode.hh \
  engine/MathMLNormalizingContainerElement.hh \
  engine/MathMLNumberElement.hh \
  engine/MathMLOperatorElement.hh \
  engine/MathMLPaddedElement.hh \
  engine/MathMLPhantomElement.hh \
  engine/MathMLRadicalElement.hh \
  engine/MathMLRowElement.hh \
  engine/MathMLScriptElement.hh \
  engine/MathMLSpaceElement.hh \
  engine/MathMLStringLitElement.hh \
  engine/MathMLStringNode.hh \
  engine/MathMLStyleElement.hh \
  engine/MathMLTableCellElement.hh \
  engine/MathMLTableContentFactory.hh \
  engine/MathMLTableElement.hh \
  engine/MathMLTableFormatter.hh \
  engine/MathMLTextElement.hh \
  engine/MathMLTextNode.hh \
  engine/MathMLTokenElement.hh \
  engine/MathMLUnderOverElement.hh \
  engine/MathMLValueConversion.hh \
  engine/mathVariantAux.hh \
  engine/traverseAux.hh \
  $(NULL)


noinst_LTLIBRARIES += libbackend.la

libbackend_la_SOURCES = \
  backend/Area.cc \
  backend/AreaFactory.cc \
  backend/AreaId.cc \
  backend/AreaIdAux.cc \
  backend/Backend.cc \
  backend/BackgroundArea.cc \
  backend/BinContainerArea.cc \
  backend/BoxArea.cc \
  backend/BoxedLayoutArea.cc \
  backend/ColorArea.cc \
  backend/CombinedGlyphArea.cc \
  backend/FormattingContext.cc \
  backend/GlyphArea.cc \
  backend/GlyphStringArea.cc \
  backend/GlyphWrapperArea.cc \
  backend/GraphicDevice.cc \
  backend/HideArea.cc \
  backend/HorizontalArrayArea.cc \
  backend/HorizontalFillerArea.cc \
  backend/HorizontalSpaceArea.cc \
  backend/IdArea.cc \
  backend/IgnoreArea.cc	\
  backend/InkArea.cc \
  backend/LinearContainerArea.cc \
  backend/MathFont.cc \
  backend/MathGraphicDevice.cc \
  backend/MathShaper.cc \
  backend/MathVariantMap.cc \
  backend/MathVariants.cc \
  backend/NullShaper.cc \
  backend/OverlapArrayArea.cc \
  backend/Shaper.cc \
  backend/ShaperManager.cc \
  backend/ShapingContext.cc \
  backend/ShiftArea.cc \
  backend/SimpleArea.cc \
  backend/SpaceShaper.cc \
  backend/StepArea.cc \
  backend/VerticalArrayArea.cc \
  backend/VerticalFillerArea.cc \
  backend/VerticalSpaceArea.cc \
  backend/WrapperArea.cc \
  $(NULL)

libbackend_la_LIBADD = \
  $(HARFBUZZ_LIBS) \
  $(NULL)

mathview_HEADERS += \
  backend/Area.hh \
  backend/AreaFactory.hh \
  backend/AreaId.hh \
  backend/AreaIdAux.hh \
  backend/Backend.hh \
  backend/BackgroundArea.hh \
  backend/BinContainerArea.hh \
  backend/BoxArea.hh \
  backend/BoxedLayoutArea.hh \
  backend/CachedShapedString.hh \
  backend/ColorArea.hh	\
  backend/CombinedGlyphArea.hh \
  backend/ContainerArea.hh \
  backend/FillerArea.hh \
  backend/FormattingContext.hh \
  backend/GlyphArea.hh \
  backend/GlyphSpec.hh \
  backend/GlyphStringArea.hh \
  backend/GlyphWrapperArea.hh \
  backend/GraphicDevice.hh \
  backend/HideArea.hh \
  backend/HorizontalArrayArea.hh \
  backend/HorizontalFillerArea.hh \
  backend/HorizontalSpaceArea.hh \
  backend/IdArea.hh \
  backend/IgnoreArea.hh	\
  backend/InkArea.hh \
  backend/LinearContainerArea.hh \
  backend/MathFont.hh \
  backend/MathGraphicDevice.hh \
  backend/MathShaper.hh \
  backend/MathTable.hh \
  backend/MathVariantMap.hh \
  backend/NullShaper.hh \
  backend/OverlapArrayArea.hh \
  backend/RenderingContext.hh \
  backend/SearchingContext.hh \
  backend/Shaper.hh \
  backend/ShaperManager.hh \
  backend/ShapingContext.hh \
  backend/ShiftArea.hh \
  backend/SimpleArea.hh \
  backend/SpaceShaper.hh \
  backend/StepArea.hh \
  backend/VerticalArrayArea.hh \
  backend/VerticalFillerArea.hh \
  backend/VerticalSpaceArea.hh \
  backend/WrapperArea.hh \
  $(NULL)

if HAVE_CAIRO
lib_LTLIBRARIES += libmathview_backend_cairo.la

libmathview_backend_cairo_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_backend_cairo_la_LIBADD = \
  $(CAIRO_LIBS) \
  $(NULL)

libmathview_backend_cairo_la_SOURCES = \
  backend/cairo/Cairo_Backend.cc \
  backend/cairo/Cairo_GlyphArea.cc \
  backend/cairo/Cairo_GlyphArea.hh \
  backend/cairo/Cairo_RenderingContext.cc \
  backend/cairo/Cairo_RenderingContext.hh \
  backend/cairo/Cairo_Shaper.cc \
  backend/cairo/Cairo_Shaper.hh \
  $(NULL)

mathview_HEADERS += \
  backend/cairo/Cairo_Backend.hh \
  $(NULL)
endif # HAVE_CAIRO

if HAVE_QT
lib_LTLIBRARIES += libmathview_backend_qt.la

libmathview_backend_qt_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_backend_qt_la_LIBADD = \
  $(QT_LIBS) \
  $(NULL)

libmathview_backend_qt_la_SOURCES = \
  backend/qt/Qt_Backend.cc \
  backend/qt/Qt_Shaper.cc \
  backend/qt/Qt_Shaper.hh \
  backend/qt/Qt_GlyphArea.cc \
  backend/qt/Qt_GlyphArea.hh \
  backend/qt/Qt_RenderingContext.cc \
  backend/qt/Qt_RenderingContext.hh \
  $(NULL)

mathview_HEADERS += \
  backend/qt/Qt_Backend.hh \
  $(NULL)
endif # HAVE_QT


noinst_LTLIBRARIES += libfrontend_common.la

libfrontend_common_la_SOURCES = \
  frontend/common/Builder.cc \
  $(NULL)

noinst_HEADERS += \
  frontend/common/Builder.hh \
  frontend/common/TemplateBuilder.hh \
  frontend/common/TemplateLinker.hh \
  frontend/common/TemplateRefinementContext.hh \
  frontend/common/TemplateReaderModel.hh \
  frontend/common/TemplateReaderBuilder.hh \
  frontend/common/TemplateReaderNodeIterator.hh \
  frontend/common/TemplateReaderElementIterator.hh \
  frontend/common/TemplateReaderRefinementContext.hh \
  frontend/common/Reader.hh \
  $(NULL)

mathview_HEADERS += \
  frontend/common/TemplateNodeIterator.hh \
  frontend/common/TemplateElementValidator.hh \
  frontend/common/TemplateElementIterator.hh \
  $(NULL)


if HAVE_CUSTOM_READER
noinst_LTLIBRARIES += libfrontend_custom_reader.la

libfrontend_custom_reader_la_SOURCES = \
  frontend/custom_reader/customXmlReader.cc \
  frontend/custom_reader/custom_reader_Builder.cc \
  $(NULL)

mathview_HEADERS += \
  frontend/custom_reader/c_customXmlReader.h \
  $(NULL)

noinst_HEADERS += \
  frontend/custom_reader/customXmlReader.hh \
  frontend/custom_reader/custom_reader_Model.hh \
  frontend/custom_reader/custom_reader_Builder.hh \
  $(NULL)


lib_LTLIBRARIES += libmathview_frontend_custom_reader.la

libmathview_frontend_custom_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined

libmathview_frontend_custom_reader_la_SOURCES = \
  view/custom_reader_MathView.cc \
  $(NULL)

libmathview_frontend_custom_reader_la_LIBADD = \
  libfrontend_custom_reader.la \
  libmathview.la \
  $(NULL)

mathview_HEADERS += \
  view/custom_reader_MathView.hh \
  $(NULL)


if HAVE_GTK
lib_LTLIBRARIES += libgtkmathview_custom_reader.la

libgtkmathview_custom_reader_la_SOURCES = \
  widget/gtkmathview_custom_reader.cc \
  widget/gtkmathview_marshal.c \
  $(NULL)

libgtkmathview_custom_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libgtkmathview_custom_reader_la_LIBADD = \
  $(GTK_LIBS) \
  libmathview_backend_cairo.la \
  libmathview_frontend_custom_reader.la \
  $(NULL)
endif # HAVE_GTK
endif # HAVE_CUSTOM_READER

if HAVE_LIBXML2_READER
noinst_LTLIBRARIES += libfrontend_libxml2_reader.la

libfrontend_libxml2_reader_la_LIBADD = \
  $(XML_LIBS) \
  $(NULL)

libfrontend_libxml2_reader_la_SOURCES = \
  frontend/libxml2_reader/libxmlXmlReader.cc \
  frontend/libxml2_reader/libxml2_reader_Builder.cc \
  $(NULL)

noinst_HEADERS += \
  frontend/libxml2_reader/libxmlXmlReader.hh \
  frontend/libxml2_reader/libxml2_reader_Model.hh \
  frontend/libxml2_reader/libxml2_reader_Builder.hh \
  $(NULL)


lib_LTLIBRARIES += libmathview_frontend_libxml2_reader.la

libmathview_frontend_libxml2_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined

libmathview_frontend_libxml2_reader_la_SOURCES = \
  view/libxml2_reader_MathView.cc \
  $(NULL)

libmathview_frontend_libxml2_reader_la_LIBADD = \
  $(XML_LIBS) \
  libfrontend_libxml2_reader.la \
  libmathview.la \
  $(NULL)

mathview_HEADERS += \
  view/libxml2_reader_MathView.hh \
  $(NULL)


if HAVE_GTK
lib_LTLIBRARIES += libgtkmathview_libxml2_reader.la

libgtkmathview_libxml2_reader_la_SOURCES = \
  widget/gtkmathview_libxml2_reader.cc \
  widget/gtkmathview_marshal.c \
  $(NULL)

libgtkmathview_libxml2_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libgtkmathview_libxml2_reader_la_LIBADD = \
  $(GTK_LIBS) \
  libmathview_backend_cairo.la \
  libmathview_frontend_libxml2_reader.la \
  $(NULL)
endif # HAVE_GTK
endif # HAVE_LIBXML2_READER

if HAVE_LIBXML2
noinst_LTLIBRARIES += libfrontend_libxml2.la

libfrontend_libxml2_la_LIBADD = \
  $(XML_LIBS) \
  $(NULL)

libfrontend_libxml2_la_SOURCES = \
  frontend/libxml2/libxml2_EntitiesTable.cc \
  frontend/libxml2/libxml2_EntitiesTable.hh \
  frontend/libxml2/libxml2_Model.cc \
  frontend/libxml2/libxml2_Builder.cc \
  frontend/libxml2/libxml2_Builder.hh \
  $(NULL)

mathview_HEADERS += \
  frontend/libxml2/libxml2_Model.hh \
  $(NULL)


lib_LTLIBRARIES += libmathview_frontend_libxml2.la

libmathview_frontend_libxml2_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined

libmathview_frontend_libxml2_la_SOURCES = \
  view/libxml2_MathView.cc \
  $(NULL)

libmathview_frontend_libxml2_la_LIBADD = \
  $(XML_LIBS) \
  libfrontend_libxml2.la \
  libmathview.la \
  $(NULL)

mathview_HEADERS += \
  view/libxml2_MathView.hh \
  $(NULL)


if HAVE_GTK
lib_LTLIBRARIES += libgtkmathview_libxml2.la

libgtkmathview_libxml2_la_SOURCES = \
  widget/gtkmathview_libxml2.cc \
  widget/gtkmathview_marshal.c \
  $(NULL)

libgtkmathview_libxml2_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libgtkmathview_libxml2_la_LIBADD = \
  $(GTK_LIBS) \
  libmathview_backend_cairo.la \
  libmathview_frontend_libxml2.la \
  $(NULL)
endif # HAVE_GTK

if HAVE_QT
moc_%.cc: %.hh
	$(MOC) $< -o $@

lib_LTLIBRARIES += libmathview_widget_qt.la
libmathview_widget_qt_la_SOURCES = \
  widget/QMathView.cc \
  widget/moc_QMathView.cc \
  $(NULL)

libmathview_widget_qt_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libmathview_widget_qt_la_LIBADD = \
  $(QT_LIBS) \
  $(NULL)

qtdir = $(includedir)/$(PACKAGE)/
qt_HEADERS = \
  widget/QMathView.hh \
  $(NULL)
endif # HAVE_QT
endif # HAVE_LIBXML2



if HAVE_GTK
EXTRA_DIST += \
  widget/gtkmathview_common.cc \
  widget/gtkmathview_decorators_common.cc \
  widget/marshal.list \
  $(NULL)

gtkdir = $(includedir)/$(PACKAGE)/gtk
gtk_HEADERS = \
  widget/gtkmathview.h \
  widget/gtkmathview_decorators.h \
  widget/gtkmathview_common.h \
  widget/gtkmathview_decorators_common.h \
  widget/gtkmathview_custom_reader.h \
  widget/gtkmathview_libxml2_reader.h \
  widget/gtkmathview_libxml2.h \
  $(NULL)

noinst_HEADERS += \
  widget/gtkmathview_marshal.h \
  $(NULL)

BUILT_SOURCES += \
  widget/gtkmathview_marshal.c \
  widget/gtkmathview_marshal.h \
  $(NULL)

CLEANFILES += \
  $(BUILT_SOURCES) \
  $(NULL)

widget/gtkmathview_marshal.h: widget/marshal.list
	$(AM_V_GEN)$(GLIB_GENMARSHAL) --prefix=gtkmathview_marshal $< --header > $@

widget/gtkmathview_marshal.c: widget/marshal.list
	$(AM_V_GEN)$(GLIB_GENMARSHAL) --prefix=gtkmathview_marshal $< --body > $@

endif # HAVE_GTK

AM_CPPFLAGS = \
  -I$(top_builddir)/auto \
  -I$(top_builddir)/src/widget \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/view \
  -I$(top_srcdir)/src/backend/cairo \
  -I$(top_srcdir)/src/backend/qt \
  -I$(top_srcdir)/src/backend \
  -I$(top_srcdir)/src/engine \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/custom_reader \
  -I$(top_srcdir)/src/frontend/libxml2 \
  -I$(top_srcdir)/src/frontend/libxml2_reader \
  $(CAIRO_CFLAGS) \
  $(GTK_CFLAGS) \
  $(HARFBUZZ_CFLAGS) \
  $(QT_CFLAGS) -fPIC \
  $(XML_CFLAGS) \
  -DPKGDATADIR=\""$(pkgdatadir)"\" \
  -DPKGSYSCONFDIR=\""$(sysconfdir)/@PACKAGE@"\" \
  $(NULL)

-include $(top_srcdir)/git.mk
