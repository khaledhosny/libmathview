// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
//
// This file is part of GtkMathView, a Gtk widget for MathML.
// 
// GtkMathView is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// GtkMathView is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GtkMathView; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// For details, see the GtkMathView World-Wide-Web page,
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef __MathMLFormattingEngineFactory_hh__
#define __MathMLFormattingEngineFactory_hh__

#include "MathML.hh"
#include "BoxML.hh" // TEMPORARY
#include "HashMap.hh"

class MathMLFormattingEngineFactory : public Object
{
protected:
  MathMLFormattingEngineFactory(void);
  virtual ~MathMLFormattingEngineFactory();

public:
  static SmartPtr<MathMLFormattingEngineFactory> create(void)
  { return new MathMLFormattingEngineFactory(); }

  virtual SmartPtr<MathMLElement> createElement(const SmartPtr<class MathMLView>&, const String&) const;

  virtual SmartPtr<MathMLElement> createMathElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLmathElement::create(view); }
  virtual SmartPtr<MathMLElement> createIdentifierElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLIdentifierElement::create(view); }
  virtual SmartPtr<MathMLElement> createNumberElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLNumberElement::create(view); }
  virtual SmartPtr<MathMLElement> createOperatorElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLOperatorElement::create(view); }
  virtual SmartPtr<MathMLElement> createTextElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLTextElement::create(view); }
  virtual SmartPtr<MathMLElement> createSpaceElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLSpaceElement::create(view); }
  virtual SmartPtr<MathMLElement> createStringLitElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLStringLitElement::create(view); }
  virtual SmartPtr<MathMLElement> createRowElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLRowElement::create(view); }
  virtual SmartPtr<MathMLElement> createFractionElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLFractionElement::create(view); }
  virtual SmartPtr<MathMLElement> createRadicalElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLRadicalElement::create(view); }
  virtual SmartPtr<MathMLElement> createStyleElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLStyleElement::create(view); }
  virtual SmartPtr<MathMLElement> createErrorElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLErrorElement::create(view); }
  virtual SmartPtr<MathMLElement> createPaddedElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLPaddedElement::create(view); }
  virtual SmartPtr<MathMLElement> createPhantomElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLPhantomElement::create(view); }
  virtual SmartPtr<MathMLElement> createFencedElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLFencedElement::create(view); }
  virtual SmartPtr<MathMLElement> createScriptElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLScriptElement::create(view); }
  virtual SmartPtr<MathMLElement> createUnderOverElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLUnderOverElement::create(view); }
  virtual SmartPtr<MathMLElement> createMultiScriptsElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLMultiScriptsElement::create(view); }
  virtual SmartPtr<MathMLElement> createTableElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLTableElement::create(view); }
  virtual SmartPtr<MathMLElement> createTableRowElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLTableRowElement::create(view); }
  virtual SmartPtr<MathMLElement> createLabeledTableRowElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLLabeledTableRowElement::create(view); }
  virtual SmartPtr<MathMLElement> createTableCellElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLTableCellElement::create(view); }
  virtual SmartPtr<MathMLElement> createAlignGroupElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLAlignGroupElement::create(view); }
  virtual SmartPtr<MathMLElement> createAlignMarkElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLAlignMarkElement::create(view); }
  virtual SmartPtr<MathMLElement> createActionElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLActionElement::create(view); }
  virtual SmartPtr<MathMLElement> createEncloseElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLEncloseElement::create(view); }
  virtual SmartPtr<MathMLElement> createSemanticsElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLSemanticsElement::create(view); }

  virtual SmartPtr<MathMLElement> createDummyElement(const SmartPtr<class MathMLView>& view) const
  { return MathMLDummyElement::create(view); }

  // TEMPORARY FACTORY METHODS FOR BOXML
  virtual SmartPtr<MathMLElement> createAtElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLAtElement::create(view); }
  virtual SmartPtr<MathMLElement> createGElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLGElement::create(view); }
  virtual SmartPtr<MathMLElement> createHElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLHElement::create(view); }
  virtual SmartPtr<MathMLElement> createInkElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLInkElement::create(view); }
  virtual SmartPtr<MathMLElement> createBoxMLSpaceElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLSpaceElement::create(view); }
  virtual SmartPtr<MathMLElement> createBoxMLTextElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLTextElement::create(view); }
  virtual SmartPtr<MathMLElement> createVElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLVElement::create(view); }
  virtual SmartPtr<MathMLElement> createBoxElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLboxElement::create(view); }
  virtual SmartPtr<MathMLElement> createBoxMLActionElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLActionElement::create(view); }

private:
  typedef SmartPtr<MathMLElement> (MathMLFormattingEngineFactory::* FactoryMethod)(const SmartPtr<class MathMLView>&) const;
  typedef HASH_MAP_NS::hash_map<String,FactoryMethod,StringHash,StringEq> FactoryMethodMap;
  FactoryMethodMap factoryMethodMap;  
};

#endif // __MathMLFormattingEngineFactory_hh__
