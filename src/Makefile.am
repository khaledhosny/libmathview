
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
  DrawingArea.cc \
  FontAttributes.cc \
  FontManager.cc \
  GraphicsContext.cc \
  gtkmathview.cc

libgtkmathview_la_SOURCES = \
  Clock.cc \
  Configuration.cc \
  EntitiesTable.cc \
  Logger.cc \
  Globals.cc \
  ChildList.cc \
  HorizontalLayout.cc \
  VerticalLayout.cc \
  MathMLDOMLinker.cc \
  MathMLFormattingEngineFactory.cc \
  MathMLActionElement.cc \
  MathMLAlignGroupElement.cc \
  MathMLAlignMarkElement.cc \
  MathMLAttribute.cc \
  MathMLAttributeList.cc \
  MathMLAttributeSignature.cc \
  MathMLContainerElement.cc \
  MathMLBinContainerElement.cc \
  MathMLLinearContainerElement.cc \
  MathMLDummyElement.cc \
  MathMLElement.cc \
  MathMLEmbellishment.cc \
  MathMLEncloseElement.cc \
  MathMLErrorElement.cc \
  MathMLFencedElement.cc \
  MathMLFractionElement.cc \
  MathMLFrame.cc \
  MathMLGlyphNode.cc \
  MathMLIdentifierElement.cc \
  MathMLLabeledTableRowElement.cc \
  MathMLMarkNode.cc \
  MathMLMultiScriptsElement.cc \
  MathMLNode.cc \
  MathMLNormalizingContainerElement.cc \
  MathMLNumberElement.cc \
  MathMLOperatorElement.cc \
  MathMLPaddedElement.cc \
  MathMLParseFile.cc \
  MathMLDOMLinker.cc \
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
  MathMLView.cc \
  MathMLViewContext.cc \
  MathMLmathElement.cc \
  OperatorDictionary.cc \
  Rectangle.cc \
  RenderingEnvironment.cc \
  RefinementContext.cc \
  ValueConversion.cc \
  frameAux.cc \
  gmetadom.cc \
  scaledConv.cc \
  token.cc \
  operatorAux.cc \
  tableLayout.cc \
  tableSetup.cc \
  traverseAux.cc \
  mathVariantAux.cc \
  $(GUISOURCES) \
  $(GTKSOURCES)

pkginclude_HEADERS = \
  defs.h \
  gtkmathview.h

noinst_HEADERS = \
  AFont.hh \
  Adaptors.hh \
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
  MathMLAttributeParsers.hh \
  MathMLDOMLinker.hh \
  MathMLFormattingEngineFactory.hh \
  MathMLActionElement.hh \
  MathMLAlignGroupElement.hh \
  MathMLAlignMarkElement.hh \
  MathMLAttribute.hh \
  MathMLAttributeList.hh \
  MathMLContainerElement.hh \
  MathMLBinContainerElement.hh \
  MathMLLinearContainerElement.hh \
  MathMLDummyElement.hh \
  MathMLElement.hh \
  MathMLEmbellishment.hh \
  MathMLEncloseElement.hh \
  MathMLErrorElement.hh \
  MathMLFencedElement.hh \
  MathMLFractionElement.hh \
  MathMLFrame.hh \
  MathMLGlyphNode.hh \
  MathMLIdentifierElement.hh \
  MathMLLabeledTableRowElement.hh \
  MathMLMarkNode.hh \
  MathMLMultiScriptsElement.hh \
  MathMLNode.hh \
  MathMLNormalizingContainerElement.hh \
  MathMLNumberElement.hh \
  MathMLOperatorElement.hh \
  MathMLPaddedElement.hh \
  MathMLParseFile.hh \
  MathMLDOMLinker.hh \
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
  MathMLView.hh \
  MathMLViewContext.hh \
  MathMLmathElement.hh \
  OperatorDictionary.hh \
  Rectangle.hh \
  RenderingEnvironment.hh \
  AbstractRefinementContext.hh \
  RefinementContext.hh \
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
  -I$(srcdir)/formatter/areamodel \
  -I$(srcdir)/formatter/gtk_areamodel \
  -I$(srcdir)/formatter/mathml_graphic_device \
  -I$(srcdir)/formatter/gtk_mathml_graphic_device


