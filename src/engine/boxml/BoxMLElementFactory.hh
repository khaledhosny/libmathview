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

#ifndef __BoxMLElementFactory_hh__
#define __BoxMLElementFactory_hh__

#include "BoxML.hh"
#include "HashMap.hh"
#include "BoxMLNamespaceContext.hh"
#include "ElementFactory.hh"

#define DEFINE_FACTORY_METHOD(n) SmartPtr<BoxMLElement> create##n##Element(void) const { return BoxML##n##Element::create(static_cast<BoxMLNamespaceContext*>(context)); }

class BoxMLElementFactory : public ElementFactory
{
protected:
  BoxMLElementFactory(void);
  virtual ~BoxMLElementFactory();

public:
  static SmartPtr<BoxMLElementFactory> create(void) { return new BoxMLElementFactory(); }

  SmartPtr<BoxMLElement> createBoxMLElement(const String&) const;
  virtual SmartPtr<Element> createElement(const String&) const;
  void setContext(const SmartPtr<BoxMLNamespaceContext>&);

  DEFINE_FACTORY_METHOD(At);
  DEFINE_FACTORY_METHOD(Layout);
  DEFINE_FACTORY_METHOD(H);
  DEFINE_FACTORY_METHOD(Ink);
  DEFINE_FACTORY_METHOD(Space);
  DEFINE_FACTORY_METHOD(Text);
  DEFINE_FACTORY_METHOD(V);
  DEFINE_FACTORY_METHOD(Box);
  DEFINE_FACTORY_METHOD(Action);
  DEFINE_FACTORY_METHOD(Object);

private:
  typedef SmartPtr<BoxMLElement> (BoxMLElementFactory::* FactoryMethod)(void) const;
  typedef HASH_MAP_NS::hash_map<String,FactoryMethod,StringHash,StringEq> FactoryMethodMap;
  FactoryMethodMap factoryMethodMap;  
  WeakPtr<BoxMLNamespaceContext> context;
};
  
#endif // __BoxMLElementFactory_hh__
