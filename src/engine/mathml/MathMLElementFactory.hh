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
#include "MathMLNamespaceContext.hh"
#include "ElementFactory.hh"

#define DEFINE_ELEMENT_FACTORY(n) SmartPtr<MathMLElement> create##n##Element(void) const { return MathML##n##Element::create(static_cast<MathMLNamespaceContext*>(context)); }

class MathMLElementFactory : public ElementFactory
{
protected:
  MathMLElementFactory(void);
  virtual ~MathMLElementFactory();

public:
  static SmartPtr<MathMLElementFactory> create(void) { return new MathMLElementFactory(); }

  SmartPtr<MathMLElement> createMathMLElement(const String&) const;
  virtual SmartPtr<Element> createElement(const String&) const;
  void setContext(const SmartPtr<MathMLNamespaceContext>&);

  DEFINE_ELEMENT_FACTORY(math);
  DEFINE_ELEMENT_FACTORY(Identifier);
  DEFINE_ELEMENT_FACTORY(Number);
  DEFINE_ELEMENT_FACTORY(Operator);
  DEFINE_ELEMENT_FACTORY(Text);
  DEFINE_ELEMENT_FACTORY(Space);
  DEFINE_ELEMENT_FACTORY(StringLit);
  DEFINE_ELEMENT_FACTORY(Row);
  DEFINE_ELEMENT_FACTORY(Fraction);
  DEFINE_ELEMENT_FACTORY(Radical);
  DEFINE_ELEMENT_FACTORY(Style);
  DEFINE_ELEMENT_FACTORY(Error);
  DEFINE_ELEMENT_FACTORY(Padded);
  DEFINE_ELEMENT_FACTORY(Phantom);
  DEFINE_ELEMENT_FACTORY(Fenced);
  DEFINE_ELEMENT_FACTORY(Script);
  DEFINE_ELEMENT_FACTORY(UnderOver);
  DEFINE_ELEMENT_FACTORY(MultiScripts);
  DEFINE_ELEMENT_FACTORY(Table);
  DEFINE_ELEMENT_FACTORY(TableRow);
  DEFINE_ELEMENT_FACTORY(LabeledTableRow);
  DEFINE_ELEMENT_FACTORY(TableCell);
  DEFINE_ELEMENT_FACTORY(AlignGroup);
  DEFINE_ELEMENT_FACTORY(AlignMark);
  DEFINE_ELEMENT_FACTORY(Action);
  DEFINE_ELEMENT_FACTORY(Enclose);
  DEFINE_ELEMENT_FACTORY(Semantics);
  DEFINE_ELEMENT_FACTORY(Dummy);

private:
  typedef SmartPtr<MathMLElement> (MathMLElementFactory::* FactoryMethod)(void) const;
  typedef HASH_MAP_NS::hash_map<String,FactoryMethod,StringHash,StringEq> FactoryMethodMap;
  FactoryMethodMap factoryMethodMap;
  WeakPtr<MathMLNamespaceContext> context;
};

#endif // __MathMLElementFactory_hh__
