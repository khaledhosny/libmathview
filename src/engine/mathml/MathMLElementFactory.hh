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

#ifndef __MathMLElementFactory_hh__
#define __MathMLElementFactory_hh__

#include "MathML.hh"
#include "HashMap.hh"
#include "ElementFactory.hh"

class MathMLElementFactory : public ElementFactory
{
protected:
  MathMLElementFactory(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLElementFactory();

public:
  static SmartPtr<MathMLElementFactory> create(const SmartPtr<class MathMLNamespaceContext>& context)
  { return new MathMLElementFactory(context); }

  virtual SmartPtr<Element> createElement(const String& name) const;

  SmartPtr<MathMLElement> createMathElement(void) const { return MathMLmathElement::create(context); }
  SmartPtr<MathMLElement> createIdentifierElement(void) const { return MathMLIdentifierElement::create(context); }
  SmartPtr<MathMLElement> createNumberElement(void) const { return MathMLNumberElement::create(context); }
  SmartPtr<MathMLElement> createOperatorElement(void) const { return MathMLOperatorElement::create(context); }
  SmartPtr<MathMLElement> createTextElement(void) const { return MathMLTextElement::create(context); }
  SmartPtr<MathMLElement> createSpaceElement(void) const { return MathMLSpaceElement::create(context); }
  SmartPtr<MathMLElement> createStringLitElement(void) const { return MathMLStringLitElement::create(context); }
  SmartPtr<MathMLElement> createRowElement(void) const { return MathMLRowElement::create(context); }
  SmartPtr<MathMLElement> createFractionElement(void) const { return MathMLFractionElement::create(context); }
  SmartPtr<MathMLElement> createRadicalElement(void) const { return MathMLRadicalElement::create(context); }
  SmartPtr<MathMLElement> createStyleElement(void) const { return MathMLStyleElement::create(context); }
  SmartPtr<MathMLElement> createErrorElement(void) const { return MathMLErrorElement::create(context); }
  SmartPtr<MathMLElement> createPaddedElement(void) const { return MathMLPaddedElement::create(context); }
  SmartPtr<MathMLElement> createPhantomElement(void) const { return MathMLPhantomElement::create(context); }
  SmartPtr<MathMLElement> createFencedElement(void) const { return MathMLFencedElement::create(context); }
  SmartPtr<MathMLElement> createScriptElement(void) const { return MathMLScriptElement::create(context); }
  SmartPtr<MathMLElement> createUnderOverElement(void) const { return MathMLUnderOverElement::create(context); }
  SmartPtr<MathMLElement> createMultiScriptsElement(void) const { return MathMLMultiScriptsElement::create(context); }
  SmartPtr<MathMLElement> createTableElement(void) const { return MathMLTableElement::create(context); }
  SmartPtr<MathMLElement> createTableRowElement(void) const { return MathMLTableRowElement::create(context); }
  SmartPtr<MathMLElement> createLabeledTableRowElement(void) const { return MathMLLabeledTableRowElement::create(context); }
  SmartPtr<MathMLElement> createTableCellElement(void) const { return MathMLTableCellElement::create(context); }
  SmartPtr<MathMLElement> createAlignGroupElement(void) const { return MathMLAlignGroupElement::create(context); }
  SmartPtr<MathMLElement> createAlignMarkElement(void) const { return MathMLAlignMarkElement::create(context); }
  SmartPtr<MathMLElement> createActionElement(void) const { return MathMLActionElement::create(context); }
  SmartPtr<MathMLElement> createEncloseElement(void) const { return MathMLEncloseElement::create(context); }
  SmartPtr<MathMLElement> createSemanticsElement(void) const { return MathMLSemanticsElement::create(context); }
  SmartPtr<MathMLElement> createDummyElement(void) const { return MathMLDummyElement::create(context); }

private:
  typedef SmartPtr<MathMLElement> (MathMLElementFactory::* FactoryMethod)(void) const;
  typedef HASH_MAP_NS::hash_map<String,FactoryMethod,StringHash,StringEq> FactoryMethodMap;
  FactoryMethodMap factoryMethodMap;  
  SmartPtr<MathMLNamespaceContext> context;
};

#endif // __MathMLElementFactory_hh__
