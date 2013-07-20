
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

libmathview_la_CPPFLAGS = -DGMV_MathView_DLL
libmathview_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined

libmathview_la_SOURCES = dummy.cc

libmathview_la_LIBADD = \
  libbackend_common.la \
  libfrontend_common.la \
  libengine_mathml.la \
  libengine_common.la \
  libcommon.la \
  -lstdc++ \
  $(NULL)



noinst_LTLIBRARIES += libcommon.la

libcommon_la_CPPFLAGS = $(NULL)
libcommon_la_CPPFLAGS += -DGMV_MathView_DLL
libcommon_la_LIBADD = \
  libmathvariants.la \
  $(GLIB_LIBS) \
  $(NULL)

libcommon_la_SOURCES = \
  common/AbstractLogger.cc \
  common/BoundingBox.cc \
  common/BoundingBoxAux.cc \
  common/Clock.cc \
  common/Configuration.cc \
  common/LengthAux.cc \
  common/Logger.cc \
  common/PointAux.cc \
  common/RGBColorAux.cc \
  common/Rectangle.cc \
  common/String.cc \
  common/StringAux.cc \
  common/StringHash.cc \
  common/Utils.cc \
  common/ValueConversion.cc \
  common/token.cc \
  $(NULL)

mathview_HEADERS += \
  common/AbstractLogger.hh \
  common/BoundingBox.hh \
  common/BoundingBoxAux.hh \
  common/Configuration.hh \
  common/Length.hh \
  common/LengthAux.hh \
  common/Logger.hh \
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
  common/Utils.hh \
  common/Value.hh \
  common/ValueConversion.hh \
  common/Variant.hh \
  common/WeakPtr.hh \
  common/defs.h \
  common/fixed.hh \
  common/gmv_defines.h \
  common/scaled.hh \
  common/scaledAux.hh \
  $(NULL)

noinst_HEADERS += \
  common/CharTraits.hh \
  common/CharTraits.icc \
  common/Clock.hh \
  common/GObjectPtr.hh \
  common/TemplateStringScanners.hh \
  common/TemplateStringParsers.hh \
  common/TokenSet.hh \
  common/for_each_if.h \
  common/token.hh \
  $(NULL)

libcommon_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  $(GLIB_CFLAGS) \
  $(NULL)



MATHVARIANTS_CC = \
  common/mathvariants/bold-fraktur.cc \
  common/mathvariants/bold-italic.cc \
  common/mathvariants/bold-sans-serif.cc \
  common/mathvariants/bold-script.cc \
  common/mathvariants/bold.cc \
  common/mathvariants/double-struck.cc \
  common/mathvariants/fraktur.cc \
  common/mathvariants/italic.cc \
  common/mathvariants/monospace.cc \
  common/mathvariants/sans-serif-bold-italic.cc \
  common/mathvariants/sans-serif-italic.cc \
  common/mathvariants/sans-serif.cc \
  common/mathvariants/script.cc \
  $(NULL)

EXTRA_DIST += \
  common/mathvariants/variant.top \
  common/mathvariants/variant.bot \
  common/mathvariants/extract.xsl \
  $(NULL)

BUILT_SOURCES += $(MATHVARIANTS_CC)

noinst_LTLIBRARIES += libmathvariants.la

libmathvariants_la_CPPFLAGS = -DGMV_MathView_DLL
libmathvariants_la_SOURCES = \
  $(MATHVARIANTS_CC) \
  common/mathvariants/MathVariantMap.cc \
  $(NULL)

mathview_HEADERS += \
  common/mathvariants/MathVariant.hh \
  common/mathvariants/MathVariantMap.hh \
  $(NULL)

libmathvariants_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common

if COND_XSLTPROC
XSLTPROC = xsltproc

# XXX: rewrite this messy shell hackery into some readible script

common/mathvariants/%_gen.cc : $(srcdir)/common/mathvariants/xml/%.xml $(srcdir)/common/mathvariants/extract.xsl
	$(AM_V_GEN)$(XSLTPROC) --novalid $(srcdir)/common/mathvariants/extract.xsl $< >$@

common/mathvariants/%.cc : common/mathvariants/%_gen.cc $(srcdir)/common/mathvariants/variant.top $(srcdir)/common/mathvariants/variant.bot
	$(AM_V_GEN)
	$(AM_V_at)cat $(srcdir)/common/mathvariants/variant.top >$@
	$(AM_V_at)$(XSLTPROC) --novalid --param temp "false()" $(srcdir)/common/mathvariants/extract.xsl $(srcdir)/common/mathvariants/xml/`basename $@ .cc`.xml >>$@
	$(AM_V_at)$(CXX) -o $(@:%.cc=%) -I$(top_builddir)/auto $<
	$(AM_V_at)echo "Char32 " >>$@
	$(AM_V_at)echo "map_variant_`basename $@ .cc | tr '-' '_'`" >>$@
	$(AM_V_at)echo "(Char32 ch)" >>$@
	$(AM_V_at)echo "{" >>$@
	$(AM_V_at)./$(@:%.cc=%) >>$@
	$(AM_V_at)echo "  return ch;" >>$@
	$(AM_V_at)echo "}" >>$@
	$(AM_V_at)rm -f $(@:%.cc=%)
	$(AM_V_at)cat $(srcdir)/common/mathvariants/variant.bot >>$@
endif # COND_XSLTPROC

EXTRA_DIST += \
  common/mathvariants/xml/bold-fraktur.xml \
  common/mathvariants/xml/bold-italic.xml \
  common/mathvariants/xml/bold-sans-serif.xml \
  common/mathvariants/xml/bold-script.xml \
  common/mathvariants/xml/bold.xml \
  common/mathvariants/xml/double-struck.xml \
  common/mathvariants/xml/fraktur.xml \
  common/mathvariants/xml/italic.xml \
  common/mathvariants/xml/monospace.xml \
  common/mathvariants/xml/sans-serif-bold-italic.xml \
  common/mathvariants/xml/sans-serif-italic.xml \
  common/mathvariants/xml/sans-serif.xml \
  common/mathvariants/xml/script.xml




noinst_LTLIBRARIES += libengine_common.la

libengine_common_la_CPPFLAGS = -DGMV_MathView_DLL

libengine_common_la_SOURCES = \
  engine/common/Attribute.cc \
  engine/common/AttributeSet.cc \
  engine/common/AttributeSignature.cc \
  engine/common/NamespaceContext.cc \
  engine/common/View.cc \
  engine/common/Node.cc \
  engine/common/Element.cc \
  $(NULL)

libengine_common_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/backend/common \
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

libengine_mathml_la_CPPFLAGS = -DGMV_MathView_DLL

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

libengine_mathml_la_CPPFLAGS += \
  -I$(top_srcdir)/auto \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/backend/common \
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

libbackend_common_la_CPPFLAGS = -DGMV_MathView_DLL

libbackend_common_la_SOURCES = \
  backend/common/Area.cc \
  backend/common/AreaFactory.cc \
  backend/common/AreaId.cc \
  backend/common/AreaIdAux.cc \
  backend/common/Backend.cc \
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

libbackend_common_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/backend/common \
  $(NULL)

libbackend_common_la_LIBADD = \
  $(NULL)

mathview_HEADERS += \
  backend/common/Area.hh \
  backend/common/AreaFactory.hh \
  backend/common/AreaId.hh \
  backend/common/AreaIdAux.hh \
  backend/common/Backend.hh \
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

if COND_GTK
lib_LTLIBRARIES += libmathview_backend_cairo.la

libmathview_backend_cairo_la_CPPFLAGS = -DGMV_BackEnd_DLL

libmathview_backend_cairo_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

libmathview_backend_cairo_la_LIBADD = \
  $(PANGO_LIBS) \
  $(CAIRO_LIBS) \
  $(NULL)

libmathview_backend_cairo_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/backend/common \
  $(PANGO_CFLAGS) \
  $(CAIRO_CFLAGS) \
  $(NULL)

libmathview_backend_cairo_la_SOURCES = \
  backend/cairo/Cairo_AreaFactory.hh \
  backend/cairo/Cairo_Backend.cc \
  backend/cairo/Cairo_BackgroundArea.cc \
  backend/cairo/Cairo_BackgroundArea.hh \
  backend/cairo/Cairo_ColorArea.cc \
  backend/cairo/Cairo_ColorArea.hh \
  backend/cairo/Cairo_InkArea.cc \
  backend/cairo/Cairo_InkArea.hh \
  backend/cairo/Cairo_MathGraphicDevice.cc \
  backend/cairo/Cairo_MathGraphicDevice.hh \
  backend/cairo/Cairo_PangoShaper.cc \
  backend/cairo/Cairo_PangoShaper.hh \
  backend/cairo/Cairo_PangoGlyphArea.cc \
  backend/cairo/Cairo_PangoGlyphArea.hh \
  backend/cairo/Cairo_PangoLayoutArea.cc \
  backend/cairo/Cairo_PangoLayoutArea.hh \
  backend/cairo/Cairo_PangoLayoutLineArea.cc \
  backend/cairo/Cairo_PangoLayoutLineArea.hh \
  backend/cairo/Cairo_RenderingContext.cc \
  backend/cairo/Cairo_RenderingContext.hh \
  backend/cairo/Cairo_WrapperArea.cc \
  backend/cairo/Cairo_WrapperArea.hh \
  $(NULL)

mathview_HEADERS += \
  backend/cairo/Cairo_Backend.hh \
  $(NULL)
endif # COND_GTK



noinst_LTLIBRARIES += libfrontend_common.la

libfrontend_common_la_CPPFLAGS = -DGMV_MathView_DLL

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
  frontend/common/TemplateSetup.hh \
  frontend/common/Reader.hh \
  $(NULL)

mathview_HEADERS += \
  frontend/common/TemplateNodeIterator.hh \
  frontend/common/TemplateElementValidator.hh \
  frontend/common/TemplateElementIterator.hh \
  $(NULL)

libfrontend_common_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/backend/common \
  $(NULL)


if COND_CUSTOM_READER
noinst_LTLIBRARIES += libfrontend_custom_reader.la

libfrontend_custom_reader_la_CPPFLAGS = -DGMV_FrontEnd_DLL

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

libfrontend_custom_reader_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/backend/common \
  $(NULL)


lib_LTLIBRARIES += libmathview_frontend_custom_reader.la

libmathview_frontend_custom_reader_la_CPPFLAGS = -DGMV_FrontEnd_DLL

libmathview_frontend_custom_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined

libmathview_frontend_custom_reader_la_SOURCES = \
  view/custom_reader_MathView.cc \
  $(NULL)

libmathview_frontend_custom_reader_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/custom_reader \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/backend/common \
  $(XML_CFLAGS) \
  $(NULL)

libmathview_frontend_custom_reader_la_LIBADD = \
  $(XML_LIBS) \
  libfrontend_libxml2_reader.la \
  libfrontend_custom_reader.la \
  libmathview.la \
  -lstdc++ \
  $(NULL)

mathview_HEADERS += \
  view/custom_reader_MathView.hh \
  $(NULL)


if COND_GTK
lib_LTLIBRARIES += libgtkmathview_custom_reader.la

libgtkmathview_custom_reader_la_CPPFLAGS = \
  -DGMV_Widget_DLL \
  -I$(top_builddir)/auto \
  -I$(top_builddir)/src/widget \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/custom_reader \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/backend/common \
  -I$(top_srcdir)/src/backend/cairo \
  -I$(top_srcdir)/src/view \
  $(GTK_CFLAGS) \
  $(XML_CFLAGS) \
  $(NULL)

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
endif # COND_GTK
endif # COND_CUSTOM_READER

if COND_LIBXML2_READER
noinst_LTLIBRARIES += libfrontend_libxml2_reader.la

libfrontend_libxml2_reader_la_CPPFLAGS = -DGMV_FrontEnd_DLL

libfrontend_libxml2_reader_la_LIBADD = \
  $(XML_LIBS) \
  $(NULL)

libfrontend_libxml2_reader_la_SOURCES = \
  frontend/libxml2_reader/libxmlXmlReader.cc \
  frontend/libxml2_reader/libxml2_reader_Builder.cc \
  frontend/libxml2_reader/libxml2_reader_Setup.cc \
  $(NULL)

noinst_HEADERS += \
  frontend/libxml2_reader/libxmlXmlReader.hh \
  frontend/libxml2_reader/libxml2_reader_Model.hh \
  frontend/libxml2_reader/libxml2_reader_Builder.hh \
  frontend/libxml2_reader/libxml2_reader_Setup.hh \
  $(NULL)

libfrontend_libxml2_reader_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/backend/common \
  $(XML_CFLAGS) \
  $(NULL)


lib_LTLIBRARIES += libmathview_frontend_libxml2_reader.la

libmathview_frontend_libxml2_reader_la_CPPFLAGS = -DGMV_FrontEnd_DLL

libmathview_frontend_libxml2_reader_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined

libmathview_frontend_libxml2_reader_la_SOURCES = \
  view/libxml2_reader_MathView.cc \
  $(NULL)

libmathview_frontend_libxml2_reader_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/libxml2_reader \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/backend/common \
  $(XML_CFLAGS) \
  $(NULL)

libmathview_frontend_libxml2_reader_la_LIBADD = \
  $(XML_LIBS) \
  libfrontend_libxml2_reader.la \
  libmathview.la \
  -lstdc++ \
  $(NULL)

mathview_HEADERS += \
  view/libxml2_reader_MathView.hh \
  $(NULL)


if COND_GTK
lib_LTLIBRARIES += libgtkmathview_libxml2_reader.la

libgtkmathview_libxml2_reader_la_CPPFLAGS = \
  -DGMV_Widget_DLL \
  -I$(top_builddir)/auto \
  -I$(top_builddir)/src/widget \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/libxml2_reader \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/backend/common \
  -I$(top_srcdir)/src/backend/cairo \
  -I$(top_srcdir)/src/view \
  $(GTK_CFLAGS) \
  $(XML_CFLAGS) \
  $(NULL)

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
endif # COND_GTK
endif # COND_LIBXML2_READER

if COND_LIBXML2
noinst_LTLIBRARIES += libfrontend_libxml2.la

libfrontend_libxml2_la_CPPFLAGS = -DGMV_FrontEnd_DLL

libfrontend_libxml2_la_LIBADD = \
  $(XML_LIBS) \
  $(NULL)

libfrontend_libxml2_la_SOURCES = \
  frontend/libxml2/libxml2_Model.cc \
  frontend/libxml2/libxml2_Setup.cc \
  frontend/libxml2/libxml2_Builder.cc \
  frontend/libxml2/libxml2_Setup.hh \
  frontend/libxml2/libxml2_Builder.hh \
  $(NULL)

mathview_HEADERS += \
  frontend/libxml2/libxml2_Model.hh \
  $(NULL)

libfrontend_libxml2_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/backend/common \
  $(XML_CFLAGS) \
  $(NULL)


lib_LTLIBRARIES += libmathview_frontend_libxml2.la

libmathview_frontend_libxml2_la_CPPFLAGS = -DGMV_FrontEnd_DLL

libmathview_frontend_libxml2_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined

libmathview_frontend_libxml2_la_SOURCES = \
  view/libxml2_MathView.cc \
  $(NULL)

libmathview_frontend_libxml2_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/libxml2 \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/backend/common \
  $(XML_CFLAGS) \
  $(NULL)

libmathview_frontend_libxml2_la_LIBADD = \
  $(XML_LIBS) \
  libfrontend_libxml2.la \
  libmathview.la \
  -lstdc++ \
  $(NULL)

mathview_HEADERS += \
  view/libxml2_MathView.hh \
  $(NULL)


if COND_GTK
lib_LTLIBRARIES += libgtkmathview_libxml2.la

libgtkmathview_libxml2_la_CPPFLAGS = \
  -DGMV_Widget_DLL \
  -I$(top_builddir)/auto \
  -I$(top_builddir)/src/widget \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/libxml2 \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/backend/common \
  -I$(top_srcdir)/src/backend/cairo \
  -I$(top_srcdir)/src/view \
  $(GTK_CFLAGS) \
  $(XML_CFLAGS) \
  $(NULL)

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
endif # COND_GTK
endif # COND_LIBXML2

if COND_GMETADOM
noinst_LTLIBRARIES += libfrontend_gmetadom.la

libfrontend_gmetadom_la_CPPFLAGS = -DGMV_FrontEnd_DLL

libfrontend_gmetadom_la_LIBADD = \
  $(DOM_LIBS) \
  $(NULL)

libfrontend_gmetadom_la_SOURCES = \
  frontend/gmetadom/MathMLEntitiesTable.cc \
  frontend/gmetadom/MathMLEntitiesTable.hh \
  frontend/gmetadom/gmetadom.hh \
  frontend/gmetadom/gmetadom_Builder.cc \
  frontend/gmetadom/gmetadom_Builder.hh \
  frontend/gmetadom/gmetadom_Model.cc \
  frontend/gmetadom/gmetadom_Setup.cc \
  frontend/gmetadom/gmetadom_Setup.hh \
  $(NULL)

mathview_HEADERS += \
  frontend/gmetadom/gmetadom_Model.hh \
  $(NULL)

libfrontend_gmetadom_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/backend/common \
  $(DOM_CFLAGS) \
  $(NULL)


lib_LTLIBRARIES += libmathview_frontend_gmetadom.la

libmathview_frontend_gmetadom_la_CPPFLAGS = -DGMV_FrontEnd_DLL

libmathview_frontend_gmetadom_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@ -no-undefined

libmathview_frontend_gmetadom_la_SOURCES = \
  view/gmetadom_MathView.cc \
  $(NULL)

libmathview_frontend_gmetadom_la_CPPFLAGS += \
  -I$(top_builddir)/auto \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/common/mathvariants \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/gmetadom \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/backend/common \
  $(DOM_CFLAGS) \
  $(NULL)

libmathview_frontend_gmetadom_la_LIBADD = \
  $(DOM_LIBS) \
  libfrontend_gmetadom.la \
  libmathview.la \
  -lstdc++ \
  $(NULL)

mathview_HEADERS += \
  view/gmetadom_MathView.hh \
  $(NULL)


mathview_HEADERS += \
  view/Init.hh \
  $(NULL)

if COND_GTK
lib_LTLIBRARIES += libgtkmathview_gmetadom.la

libgtkmathview_gmetadom_la_CPPFLAGS = \
  -DGMV_Widget_DLL \
  -I$(top_builddir)/auto \
  -I$(top_builddir)/src/widget \
  -I$(top_srcdir)/src/common \
  -I$(top_srcdir)/src/frontend/common \
  -I$(top_srcdir)/src/frontend/gmetadom \
  -I$(top_srcdir)/src/engine/common \
  -I$(top_srcdir)/src/engine/mathml \
  -I$(top_srcdir)/src/backend/common \
  -I$(top_srcdir)/src/backend/cairo \
  -I$(top_srcdir)/src/view \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  $(NULL)

libgtkmathview_gmetadom_la_SOURCES = \
  widget/gtkmathview_gmetadom.cc \
  widget/gtkmathview_marshal.c \
  $(NULL)

libgtkmathview_gmetadom_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@
libgtkmathview_gmetadom_la_LIBADD = \
  $(GTK_LIBS) \
  libmathview_backend_cairo.la \
  libmathview_frontend_gmetadom.la \
  $(NULL)
endif # COND_GTK
endif # COND_GMETADOM



if COND_GTK
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
  widget/gtkmathview_gmetadom.h

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

endif # COND_GTK


-include $(top_srcdir)/git.mk
