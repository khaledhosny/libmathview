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

#define DEFINE_FACTORY_METHOD(n) SmartPtr<MathMLElement> create##n##Element(void) const { return MathML##n##Element::create(static_cast<MathMLNamespaceContext*>(context)); }

class MathMLElementFactory : public ElementFactory
{
protected:
  MathMLElementFactory(void);
  virtual ~MathMLElementFactory();

public:
  static SmartPtr<MathMLElementFactory> create(void)
  { return new MathMLElementFactory(); }

  SmartPtr<MathMLElement> createMathMLElement(const String& name) const;
  virtual SmartPtr<Element> createElement(const String& name) const;

  DEFINE_FACTORY_METHOD(math);
  DEFINE_FACTORY_METHOD(Identifier);
  DEFINE_FACTORY_METHOD(Number);
  DEFINE_FACTORY_METHOD(Operator);
  DEFINE_FACTORY_METHOD(Text);
  DEFINE_FACTORY_METHOD(Space);
  DEFINE_FACTORY_METHOD(StringLit);
  DEFINE_FACTORY_METHOD(Row);
  DEFINE_FACTORY_METHOD(Fraction);
  DEFINE_FACTORY_METHOD(Radical);
  DEFINE_FACTORY_METHOD(Style);
  DEFINE_FACTORY_METHOD(Error);
  DEFINE_FACTORY_METHOD(Padded);
  DEFINE_FACTORY_METHOD(Phantom);
  DEFINE_FACTORY_METHOD(Fenced);
  DEFINE_FACTORY_METHOD(Script);
  DEFINE_FACTORY_METHOD(UnderOver);
  DEFINE_FACTORY_METHOD(MultiScripts);
  DEFINE_FACTORY_METHOD(Table);
  DEFINE_FACTORY_METHOD(TableRow);
  DEFINE_FACTORY_METHOD(LabeledTableRow);
  DEFINE_FACTORY_METHOD(TableCell);
  DEFINE_FACTORY_METHOD(AlignGroup);
  DEFINE_FACTORY_METHOD(AlignMark);
  DEFINE_FACTORY_METHOD(Action);
  DEFINE_FACTORY_METHOD(Enclose);
  DEFINE_FACTORY_METHOD(Semantics);
  DEFINE_FACTORY_METHOD(Dummy);

  void setContext(const SmartPtr<MathMLNamespaceContext>&);

private:
  typedef SmartPtr<MathMLElement> (MathMLElementFactory::* FactoryMethod)(void) const;
  typedef HASH_MAP_NS::hash_map<String,FactoryMethod,StringHash,StringEq> FactoryMethodMap;
  FactoryMethodMap factoryMethodMap;  
  WeakPtr<MathMLNamespaceContext> context;
};

#endif // __MathMLElementFactory_hh__
