
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
  libbackend_common.la \
  libfrontend_common.la \
  libengine_mathml.la \
  libengine_common.la \
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



noinst_LTLIBRARIES += libengine_common.la

libengine_common_la_SOURCES = \
  engine/common/Attribute.cc \
  engine/common/AttributeSet.cc \
  engine/common/AttributeSignature.cc \
  engine/common/NamespaceContext.cc \
  engine/common/View.cc \
  engine/common/Node.cc \
  engine/common/Element.cc \
  $(NULL)

mathview_HEADERS += \
  engine/common/NamespaceContext.hh \
  engine/common/Element.hh \
  engine/common/Node.hh \
  engine/common/View.hh \
  $(NULL)

noinst_HEADERS += \
  engine/common/Adapters.hh \
  engine/common/Attribute.hh \
  engine/common/AttributeSet.hh \
  engine/common/AttributeSignature.hh \
  engine/common/BinContainerTemplate.hh \
  engine/common/CommonStringParsers.hh \
  engine/common/ElementFactory.hh \
  engine/common/LinearContainerTemplate.hh \
  $(NULL)



noinst_LTLIBRARIES += libengine_mathml.la

libengine_mathml_la_SOURCES = \
  engine/mathml/MathMLActionElement.cc \
  engine/mathml/MathMLAlignGroupElement.cc \
  engine/mathml/MathMLAlignMarkElement.cc \
  engine/mathml/MathMLAttributeSignatures.cc \
  engine/mathml/MathMLBinContainerElement.cc \
  engine/mathml/MathMLContainerElement.cc \
  engine/mathml/MathMLDummyElement.cc \
  engine/mathml/MathMLElement.cc \
  engine/mathml/MathMLEmbellishment.cc \
  engine/mathml/MathMLEncloseElement.cc \
  engine/mathml/MathMLErrorElement.cc \
  engine/mathml/MathMLFractionElement.cc \
  engine/mathml/MathMLFunctionApplicationNode.cc \
  engine/mathml/MathMLGlyphNode.cc \
  engine/mathml/MathMLIdentifierElement.cc \
  engine/mathml/MathMLInferredRowElement.cc \
  engine/mathml/MathMLInvisibleTimesNode.cc \
  engine/mathml/MathMLLinearContainerElement.cc \
  engine/mathml/MathMLMarkNode.cc \
  engine/mathml/MathMLMultiScriptsElement.cc \
  engine/mathml/MathMLNamespaceContext.cc \
  engine/mathml/MathMLNode.cc \
  engine/mathml/MathMLNormalizingContainerElement.cc \
  engine/mathml/MathMLNumberElement.cc \
  engine/mathml/MathMLOperatorDictionary.cc \
  engine/mathml/MathMLOperatorElement.cc \
  engine/mathml/MathMLPaddedElement.cc \
  engine/mathml/MathMLPhantomElement.cc \
  engine/mathml/MathMLRadicalElement.cc \
  engine/mathml/MathMLRowElement.cc \
  engine/mathml/MathMLScriptElement.cc \
  engine/mathml/MathMLSpaceElement.cc \
  engine/mathml/MathMLStringLitElement.cc \
  engine/mathml/MathMLStringNode.cc \
  engine/mathml/MathMLStyleElement.cc \
  engine/mathml/MathMLTableCellElement.cc \
  engine/mathml/MathMLTableContentFactory.cc \
  engine/mathml/MathMLTableElement.cc \
  engine/mathml/MathMLTableFormatter.cc \
  engine/mathml/MathMLTextElement.cc \
  engine/mathml/MathMLTextNode.cc \
  engine/mathml/MathMLTokenElement.cc \
  engine/mathml/MathMLUnderOverElement.cc \
  engine/mathml/MathMLValueConversion.cc \
  engine/mathml/MathMLmathElement.cc \
  engine/mathml/mathVariantAux.cc \
  engine/mathml/traverseAux.cc \
  $(NULL)

mathview_HEADERS += \
  engine/mathml/MathMLNamespaceContext.hh \
  engine/mathml/MathMLOperatorDictionary.hh \
  engine/mathml/MathMLElement.hh \
  $(NULL)

noinst_HEADERS += \
  engine/mathml/MathML.hh \
  engine/mathml/MathMLActionElement.hh \
  engine/mathml/MathMLAdapters.hh \
  engine/mathml/MathMLAlignGroupElement.hh \
  engine/mathml/MathMLAlignMarkElement.hh \
  engine/mathml/MathMLAttributeParsers.hh \
  engine/mathml/MathMLAttributeParsers.hh \
  engine/mathml/MathMLAttributeSignatures.hh \
  engine/mathml/MathMLBinContainerElement.hh \
  engine/mathml/MathMLContainerElement.hh \
  engine/mathml/MathMLDummyElement.hh \
  engine/mathml/MathMLEmbellishment.hh \
  engine/mathml/MathMLEncloseElement.hh \
  engine/mathml/MathMLErrorElement.hh \
  engine/mathml/MathMLFractionElement.hh \
  engine/mathml/MathMLFunctionApplicationNode.hh \
  engine/mathml/MathMLGlyphNode.hh \
  engine/mathml/MathMLIdentifierElement.hh \
  engine/mathml/MathMLInferredRowElement.hh \
  engine/mathml/MathMLInvisibleTimesNode.hh \
  engine/mathml/MathMLLinearContainerElement.hh \
  engine/mathml/MathMLMarkNode.hh \
  engine/mathml/MathMLMultiScriptsElement.hh \
  engine/mathml/MathMLNode.hh \
  engine/mathml/MathMLNormalizingContainerElement.hh \
  engine/mathml/MathMLNumberElement.hh \
  engine/mathml/MathMLOperatorElement.hh \
  engine/mathml/MathMLPaddedElement.hh \
  engine/mathml/MathMLPhantomElement.hh \
  engine/mathml/MathMLRadicalElement.hh \
  engine/mathml/MathMLRowElement.hh \
  engine/mathml/MathMLScriptElement.hh \
  engine/mathml/MathMLSpaceElement.hh \
  engine/mathml/MathMLStringLitElement.hh \
  engine/mathml/MathMLStringNode.hh \
  engine/mathml/MathMLStyleElement.hh \
  engine/mathml/MathMLTableCellElement.hh \
  engine/mathml/MathMLTableContentFactory.hh \
  engine/mathml/MathMLTableElement.hh \
  engine/mathml/MathMLTableFormatter.hh \
  engine/mathml/MathMLTextElement.hh \
  engine/mathml/MathMLTextNode.hh \
  engine/mathml/MathMLTokenElement.hh \
  engine/mathml/MathMLUnderOverElement.hh \
  engine/mathml/MathMLValueConversion.hh \
  engine/mathml/MathMLmathElement.hh \
  engine/mathml/mathVariantAux.hh \
  engine/mathml/traverseAux.hh \
  $(NULL)


noinst_LTLIBRARIES += libbackend_common.la

libbackend_common_la_SOURCES = \
  backend/common/Area.cc \
  backend/common/AreaFactory.cc \
  backend/common/AreaId.cc \
  backend/common/AreaIdAux.cc \
  backend/common/Backend.cc \
  backend/common/BackgroundArea.cc \
  backend/common/BinContainerArea.cc \
  backend/common/BoxArea.cc \
  backend/common/BoxedLayoutArea.cc \
  backend/common/ColorArea.cc \
  backend/common/CombinedGlyphArea.cc \
  backend/common/FormattingContext.cc \
  backend/common/GlyphArea.cc \
  backend/common/GlyphStringArea.cc \
  backend/common/GlyphWrapperArea.cc \
  backend/common/GraphicDevice.cc \
  backend/common/HideArea.cc \
  backend/common/HorizontalArrayArea.cc \
  backend/common/HorizontalFillerArea.cc \
  backend/common/HorizontalSpaceArea.cc \
  backend/common/IdArea.cc \
  backend/common/IgnoreArea.cc	\
  backend/common/InkArea.cc \
  backend/common/LinearContainerArea.cc \
  backend/common/MathFont.cc \
  backend/common/MathGraphicDevice.cc \
  backend/common/MathShaper.cc \
  backend/common/MathVariantMap.cc \
  backend/common/MathVariants.cc \
  backend/common/NullShaper.cc \
  backend/common/OverlapArrayArea.cc \
  backend/common/Shaper.cc \
  backend/common/ShaperManager.cc \
  backend/common/ShapingContext.cc \
  backend/common/ShiftArea.cc \
  backend/common/SimpleArea.cc \
  backend/common/SpaceShaper.cc \
  backend/common/StepArea.cc \
  backend/common/VerticalArrayArea.cc \
  backend/common/VerticalFillerArea.cc \
  backend/common/VerticalSpaceArea.cc \
  backend/common/WrapperArea.cc \
  $(NULL)

libbackend_common_la_LIBADD = \
  $(NULL)

mathview_HEADERS += \
  backend/common/Area.hh \
  backend/common/AreaFactory.hh \
  backend/common/AreaId.hh \
  backend/common/AreaIdAux.hh \
  backend/common/Backend.hh \
  backend/common/BackgroundArea.hh \
  backend/common/BinContainerArea.hh \
  backend/common/BoxArea.hh \
  backend/common/BoxedLayoutArea.hh \
  backend/common/CachedShapedString.hh \
  backend/common/ColorArea.hh	\
  backend/common/CombinedGlyphArea.hh \
  backend/common/ContainerArea.hh \
  backend/common/FillerArea.hh \
  backend/common/FormattingContext.hh \
  backend/common/GlyphArea.hh \
  backend/common/GlyphSpec.hh \
  backend/common/GlyphStringArea.hh \
  backend/common/GlyphWrapperArea.hh \
  backend/common/GraphicDevice.hh \
  backend/common/HideArea.hh \
  backend/common/HorizontalArrayArea.hh \
  backend/common/HorizontalFillerArea.hh \
  backend/common/HorizontalSpaceArea.hh \
  backend/common/IdArea.hh \
  backend/common/IgnoreArea.hh	\
  backend/common/InkArea.hh \
  backend/common/LinearContainerArea.hh \
  backend/common/MathFont.hh \
  backend/common/MathGraphicDevice.hh \
  backend/common/MathShaper.hh \
  backend/common/MathTable.hh \
  backend/common/MathVariantMap.hh \
  backend/common/NullShaper.hh \
  backend/common/OverlapArrayArea.hh \
  backend/common/RenderingContext.hh \
  backend/common/SearchingContext.hh \
  backend/common/Shaper.hh \
  backend/common/ShaperManager.hh \
  backend/common/ShapingContext.hh \
  backend/common/ShiftArea.hh \
  backend/common/SimpleArea.hh \
  backend/common/SpaceShaper.hh \
  backend/common/StepArea.hh \
  backend/common/VerticalArrayArea.hh \
  backend/common/VerticalFillerArea.hh \
  backend/common/VerticalSpaceArea.hh \
  backend/common/WrapperArea.hh \
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
  -I$(top_srcdir)/src/backend/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/custom_reader \
  -I$(top_srcdir)/src/frontend/libxml2 \
  -I$(top_srcdir)/src/frontend/libxml2_reader \
  $(CAIRO_CFLAGS) \
  $(GTK_CFLAGS) \
  $(QT_CFLAGS) -fPIC \
  $(XML_CFLAGS) \
  -DPKGDATADIR=\""$(pkgdatadir)"\" \
  -DPKGSYSCONFDIR=\""$(sysconfdir)/@PACKAGE@"\" \
  $(NULL)

-include $(top_srcdir)/git.mk
