
lib_LTLIBRARIES = libgtkmathview.la

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
  BoundingBox.cc \
  Char.cc \
  Clock.cc \
  Configuration.cc \
  DocumentElement.cc \
  EntitiesTable.cc \
  FontifiedString.cc \
  Layout.cc \
  Logger.cc \
  MathEngine.cc \
  MathMLActionElement.cc \
  MathMLAlignGroupElement.cc \
  MathMLAlignMarkElement.cc \
  MathMLApplyFunctionNode.cc \
  MathMLAttribute.cc \
  MathMLAttributeList.cc \
  MathMLCharNode.cc \
  MathMLCombinedCharNode.cc \
  MathMLContainerElement.cc \
  MathMLDocument.cc \
  MathMLDummyElement.cc \
  MathMLElement.cc \
  MathMLEmbellishedOperatorElement.cc \
  MathMLEncloseElement.cc \
  MathMLErrorElement.cc \
  MathMLFencedElement.cc \
  MathMLFractionElement.cc \
  MathMLFrame.cc \
  MathMLGlyphNode.cc \
  MathMLHorizBarNode.cc \
  MathMLInvisibleTimesNode.cc \
  MathMLMarkNode.cc \
  MathMLMultiScriptsElement.cc \
  MathMLNode.cc \
  MathMLNormalizingContainerElement.cc \
  MathMLObject.cc \
  MathMLOperatorElement.cc \
  MathMLPaddedElement.cc \
  MathMLParseFile.cc \
  MathMLPhantomElement.cc \
  MathMLRadicalElement.cc \
  MathMLRowElement.cc \
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
  MathMLizer.cc \
  Rectangle.cc \
  RenderingEnvironment.cc \
  Shape.cc \
  ShapeFactory.cc \
  String.cc \
  StringFactory.cc \
  StringTokenizer.cc \
  StringUnicode.cc \
  UnitValue.cc \
  Value.cc \
  ValueConversion.cc \
  ValueSequence.cc \
  allocTextNode.cc \
  frameAux.cc \
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
  AFont.hh \
  Array.hh \
  Array.icc \
  AttributeParser.hh \
  AttributeSignature.hh \
  BoundingBox.hh \
  Char.hh \
  CharMap.hh \
  CharMapper.hh \
  Clock.hh \
  Configuration.hh \
  Container.hh \
  Container.icc \
  Coords.hh \
  DocumentElement.hh \
  DrawingArea.hh \
  EntitiesTable.hh \
  FontifiedString.hh \
  FontAttributes.hh \
  FontManager.hh \
  GraphicsContext.hh \
  Iterator.hh \
  Layout.hh \
  Logger.hh \
  MathML.hh \
  MathEngine.hh \
  MathMLActionElement.hh \
  MathMLAlignGroupElement.hh \
  MathMLAlignMarkElement.hh \
  MathMLApplyFunctionNode.hh \
  MathMLAttribute.hh \
  MathMLAttributeList.hh \
  MathMLCharNode.hh \
  MathMLCombinedCharNode.hh \
  MathMLContainerElement.hh \
  MathMLDocument.hh \
  MathMLDummyElement.hh \
  MathMLElement.hh \
  MathMLEmbellishedOperatorElement.hh \
  MathMLEncloseElement.hh \
  MathMLErrorElement.hh \
  MathMLFencedElement.hh \
  MathMLFractionElement.hh \
  MathMLFrame.hh \
  MathMLGlyphNode.hh \
  MathMLHorizBarNode.hh \
  MathMLInvisibleTimesNode.hh \
  MathMLMarkNode.hh \
  MathMLMultiScriptsElement.hh \
  MathMLNode.hh \
  MathMLNormalizingContainerElement.hh \
  MathMLObject.hh \
  MathMLOperatorElement.hh \
  MathMLPaddedElement.hh \
  MathMLParseFile.hh \
  MathMLPhantomElement.hh \
  MathMLRadicalElement.hh \
  MathMLRowElement.hh \
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
  MathMLizer.hh \
  RGBValue.hh \
  Rectangle.hh \
  RenderingEnvironment.hh \
  Shape.hh \
  ShapeFactory.hh \
  Stack.hh \
  String.hh \
  StringFactory.hh \
  StringTokenizer.hh \
  StringU.hh \
  StringU.icc \
  StringUnicode.hh \
  UnitValue.hh \
  Value.hh \
  ValueConversion.hh \
  ValueSequence.hh \
  allocTextNode.hh \
  frameAux.hh \
  keyword.hh \
  operatorAux.hh \
  stringAux.hh \
  traverseAux.hh \
  mathVariantAux.hh \
  scaled.hh \
  unidefs.h \
  $(GTKHEADERS) \
  $(T1HEADERS) \
  $(PSHEADERS)

INCLUDES = -I$(srcdir)/../auto/

