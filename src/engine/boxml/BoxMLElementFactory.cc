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

#include <config.h>

#include "BoxMLNamespaceContext.hh"
#include "Linker.hh"
#include "BoxMLElementFactory.hh"

BoxMLElementFactory::BoxMLElementFactory()
{
  static struct
  {
    String tag;
    FactoryMethod create;
  } tab[] = {
    { "at",            &BoxMLElementFactory::createAtElement },
    { "layout",        &BoxMLElementFactory::createLayoutElement },
    { "h",             &BoxMLElementFactory::createHElement },
    { "ink",           &BoxMLElementFactory::createInkElement },
    { "space",         &BoxMLElementFactory::createSpaceElement },
    { "text",          &BoxMLElementFactory::createTextElement },
    { "v",             &BoxMLElementFactory::createVElement },
    { "box",           &BoxMLElementFactory::createboxElement },
    { "action",        &BoxMLElementFactory::createActionElement },
    { "obj",           &BoxMLElementFactory::createObjectElement },
    { "par",           &BoxMLElementFactory::createParagraphElement },
    { "",              0 }
  };

  for (unsigned i = 0; tab[i].create; i++)
    factoryMethodMap[tab[i].tag] = tab[i].create;
}

BoxMLElementFactory::~BoxMLElementFactory()
{ }

SmartPtr<BoxMLElement>
BoxMLElementFactory::createBoxMLElement(const String& name) const
{
  FactoryMethodMap::const_iterator p = factoryMethodMap.find(name);
  if (p != factoryMethodMap.end())
    return (this->*((*p).second))();
  else
    //return createDummyElement();
    return 0;
}

SmartPtr<Element>
BoxMLElementFactory::createElement(const String& name) const
{ return createBoxMLElement(name); }

void
BoxMLElementFactory::setContext(const SmartPtr<BoxMLNamespaceContext>& c)
{
  context = static_cast<BoxMLNamespaceContext*>(c);
}
