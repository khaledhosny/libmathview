
SUBDIRS = formatter

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
  $(T1_LIBS) \
  $(top_builddir)/src/formatter/gtk_mathml_graphic_device/libgtk_mathml_graphic_device.la

libgtkmathview_la_LDFLAGS = -version-info @MATHVIEW_VERSION_INFO@

GTKSOURCES = \
  Gtk_DrawingArea.cc \
  Gtk_Font.cc \
  Gtk_FontManager.cc \
  Gtk_GraphicsContext.cc

GTKHEADERS = \
  Gtk_DrawingArea.hh \
  Gtk_Font.hh \
  Gtk_FontManager.hh \
  Gtk_GraphicsContext.hh

T1SOURCES = \
  T1_Font.cc \
  T1_FontManager.cc \
  T1_Gtk_DrawingArea.cc

T1HEADERS = \
  T1_Font.hh \
  T1_FontManager.hh \
  T1_Gtk_DrawingArea.hh

PSSOURCES = \
  PS_T1_Font.cc \
  PS_DrawingArea.cc \
  PS_T1_FontManager.cc

PSHEADERS = \
  PS_T1_Font.hh \
  PS_DrawingArea.hh \
  PS_T1_FontManager.hh

GUISOURCES = \
  AFont.cc \
  CharMap.cc \
  CharMapper.cc \
  DrawingArea.cc \
  FontAttributes.cc \
  FontManager.cc \
  GraphicsContext.cc \
  gtkmathview.cc

libgtkmathview_la_SOURCES = \
  AttributeParser.cc \
  AttributeSignature.cc \
  Char.cc \
  Clock.cc \
  Configuration.cc \
  EntitiesTable.cc \
  Logger.cc \
  Globals.cc \
  ChildList.cc \
  HorizontalLayout.cc \
  VerticalLayout.cc \
  MathMLRenderingEngine.cc \
  MathMLActionElement.cc \
  MathMLAlignGroupElement.cc \
  MathMLAlignMarkElement.cc \
  MathMLApplyFunctionNode.cc \
  MathMLAttribute.cc \
  MathMLAttributeList.cc \
  MathMLCharNode.cc \
  MathMLCombinedCharNode.cc \
  MathMLContainerElement.cc \
  MathMLBinContainerElement.cc \
  MathMLLinearContainerElement.cc \
  MathMLDocument.cc \
  MathMLDummyElement.cc \
  MathMLElement.cc \
  MathMLEmbellishment.cc \
  MathMLEncloseElement.cc \
  MathMLErrorElement.cc \
  MathMLFencedElement.cc \
  MathMLFractionElement.cc \
  MathMLFrame.cc \
  MathMLGlyphNode.cc \
  MathMLHorizBarNode.cc \
  MathMLIdentifierElement.cc \
  MathMLInvisibleTimesNode.cc \
  MathMLLabeledTableRowElement.cc \
  MathMLMarkNode.cc \
  MathMLMultiScriptsElement.cc \
  MathMLNode.cc \
  MathMLNormalizingContainerElement.cc \
  MathMLNumberElement.cc \
  MathMLObject.cc \
  MathMLOperatorElement.cc \
  MathMLPaddedElement.cc \
  MathMLParseFile.cc \
  MathMLPhantomElement.cc \
  MathMLRadicalElement.cc \
  MathMLRowElement.cc \
  MathMLBreakableRowElement.cc \
  MathMLScriptCommonElement.cc \
  MathMLScriptElement.cc \
  MathMLSemanticsElement.cc \
  MathMLSpaceElement.cc \
  MathMLSpaceNode.cc \
  MathMLStringLitElement.cc \
  MathMLStringNode.cc \
  MathMLStyleElement.cc \
  MathMLTableCellElement.cc \
  MathMLTableElement.cc \
  MathMLTableRowElement.cc \
  MathMLTextElement.cc \
  MathMLTextNode.cc \
  MathMLTokenElement.cc \
  MathMLUnderOverElement.cc \
  MathMLmathElement.cc \
  OperatorDictionary.cc \
  Rectangle.cc \
  RenderingEnvironment.cc \
  String.cc \
  StringFactory.cc \
  StringTokenizer.cc \
  StringUnicode.cc \
  UnitValue.cc \
  ValueConversion.cc \
  allocTextNode.cc \
  frameAux.cc \
  gmetadom.cc \
  keyword.cc \
  operatorAux.cc \
  stringAux.cc \
  tableLayout.cc \
  tableSetup.cc \
  traverseAux.cc \
  mathVariantAux.cc \
  $(GUISOURCES) \
  $(GTKSOURCES) \
  $(T1SOURCES) \
  $(PSSOURCES)

pkginclude_HEADERS = \
  defs.h \
  gtkmathview.h

noinst_HEADERS = \
  Ptr.hh \
  AFont.hh \
  Adaptors.hh \
  AttributeParser.hh \
  AttributeSignature.hh \
  Char.hh \
  CharMap.hh \
  CharMapper.hh \
  Clock.hh \
  Configuration.hh \
  Coords.hh \
  DrawingArea.hh \
  EntitiesTable.hh \
  FontAttributes.hh \
  FontManager.hh \
  GraphicsContext.hh \
  Logger.hh \
  Globals.hh \
  MathML.hh \
  ChildList.hh \
  HorizontalLayout.hh \
  VerticalLayout.hh \
  FormattingContext.hh \
  MathMLRenderingEngine.hh \
  MathMLActionElement.hh \
  MathMLAlignGroupElement.hh \
  MathMLAlignMarkElement.hh \
  MathMLApplyFunctionNode.hh \
  MathMLAttribute.hh \
  MathMLAttributeList.hh \
  MathMLCharNode.hh \
  MathMLCombinedCharNode.hh \
  MathMLContainerElement.hh \
  MathMLBinContainerElement.hh \
  MathMLLinearContainerElement.hh \
  MathMLDocument.hh \
  MathMLDummyElement.hh \
  MathMLElement.hh \
  MathMLEmbellishment.hh \
  MathMLEncloseElement.hh \
  MathMLErrorElement.hh \
  MathMLFencedElement.hh \
  MathMLFractionElement.hh \
  MathMLFrame.hh \
  MathMLGlyphNode.hh \
  MathMLHorizBarNode.hh \
  MathMLIdentifierElement.hh \
  MathMLInvisibleTimesNode.hh \
  MathMLLabeledTableRowElement.hh \
  MathMLMarkNode.hh \
  MathMLMultiScriptsElement.hh \
  MathMLNode.hh \
  MathMLNormalizingContainerElement.hh \
  MathMLNumberElement.hh \
  MathMLObject.hh \
  MathMLOperatorElement.hh \
  MathMLPaddedElement.hh \
  MathMLParseFile.hh \
  MathMLPhantomElement.hh \
  MathMLRadicalElement.hh \
  MathMLRowElement.hh \
  MathMLBreakableRowElement.hh \
  MathMLScriptCommonElement.hh \
  MathMLScriptElement.hh \
  MathMLSemanticsElement.hh \
  MathMLSpaceElement.hh \
  MathMLSpaceNode.hh \
  MathMLStringLitElement.hh \
  MathMLStringNode.hh \
  MathMLStyleElement.hh \
  MathMLTableCellElement.hh \
  MathMLTableElement.hh \
  MathMLTableRowElement.hh \
  MathMLTextElement.hh \
  MathMLTextNode.hh \
  MathMLTokenElement.hh \
  MathMLUnderOverElement.hh \
  MathMLmathElement.hh \
  MultiString.hh \
  OperatorDictionary.hh \
  RGBValue.hh \
  Rectangle.hh \
  RenderingEnvironment.hh \
  String.hh \
  StringFactory.hh \
  StringTokenizer.hh \
  StringU.hh \
  StringU.icc \
  StringUnicode.hh \
  UnitValue.hh \
  ValueConversion.hh \
  allocTextNode.hh \
  frameAux.hh \
  gmetadom.hh \
  keyword.hh \
  operatorAux.hh \
  stringAux.hh \
  traverseAux.hh \
  mathVariantAux.hh \
  scaledConv.hh \
  for_each_if.h \
  unidefs.h \
  $(GTKHEADERS) \
  $(T1HEADERS) \
  $(PSHEADERS)

INCLUDES = \
  $(GTK_CFLAGS) \
  $(DOM_CFLAGS) \
  $(T1_CFLAGS) \
  -I$(top_builddir) \
  -I$(top_srcdir)/auto/ \
  -I$(srcdir)/formatter/basictypes \
  -I$(srcdir)/formatter/areamodel \
  -I$(srcdir)/formatter/gtk_areamodel \
  -I$(srcdir)/formatter/mathml_graphic_device \
  -I$(srcdir)/formatter/gtk_mathml_graphic_device


