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

#include "MathMLView.hh"
#include "Linker.hh"
#include "MathMLElementFactory.hh"

MathMLElementFactory::MathMLElementFactory(const SmartPtr<MathMLView>& v)
  : view(v)
{
  static struct
  {
    String tag;
    FactoryMethod create;
  } tab[] = {
    { "math",          &MathMLElementFactory::createMathElement },
    { "mi",            &MathMLElementFactory::createIdentifierElement },
    { "mn",            &MathMLElementFactory::createNumberElement },
    { "mo",            &MathMLElementFactory::createOperatorElement },
    { "mtext",         &MathMLElementFactory::createTextElement },
    { "mspace",        &MathMLElementFactory::createSpaceElement },
    { "ms",            &MathMLElementFactory::createStringLitElement },
    { "mrow",          &MathMLElementFactory::createRowElement },
    { "mfrac",         &MathMLElementFactory::createFractionElement },
    { "msqrt",         &MathMLElementFactory::createRadicalElement },
    { "mroot",         &MathMLElementFactory::createRadicalElement },
    { "mstyle",        &MathMLElementFactory::createStyleElement },
    { "merror",        &MathMLElementFactory::createErrorElement },
    { "mpadded",       &MathMLElementFactory::createPaddedElement },
    { "mphantom",      &MathMLElementFactory::createPhantomElement },
    { "mfenced",       &MathMLElementFactory::createFencedElement },
    { "msub",          &MathMLElementFactory::createScriptElement },
    { "msup",          &MathMLElementFactory::createScriptElement },
    { "msubsup",       &MathMLElementFactory::createScriptElement },
    { "munder",        &MathMLElementFactory::createUnderOverElement },
    { "mover",         &MathMLElementFactory::createUnderOverElement },
    { "munderover",    &MathMLElementFactory::createUnderOverElement },
    { "mmultiscripts", &MathMLElementFactory::createMultiScriptsElement },
    { "mtable",        &MathMLElementFactory::createTableElement },
    { "mtr",           &MathMLElementFactory::createTableRowElement },
    { "mlabeledtr",    &MathMLElementFactory::createLabeledTableRowElement },
    { "mtd",           &MathMLElementFactory::createTableCellElement },
    { "maligngroup",   &MathMLElementFactory::createAlignGroupElement },
    { "malignmark",    &MathMLElementFactory::createAlignMarkElement },
    { "maction",       &MathMLElementFactory::createActionElement },
    { "menclose",      &MathMLElementFactory::createEncloseElement },
    { "semantics",     &MathMLElementFactory::createSemanticsElement },
#if 0
    // TEMPORARY FACTORY METHODS FOR BOXML
    { "at",            &MathMLElementFactory::createAtElement },
    { "g",             &MathMLElementFactory::createGElement },
    { "h",             &MathMLElementFactory::createHElement },
    { "ink",           &MathMLElementFactory::createInkElement },
    { "space",         &MathMLElementFactory::createBoxMLSpaceElement },
    { "text",          &MathMLElementFactory::createBoxMLTextElement },
    { "v",             &MathMLElementFactory::createVElement },
    { "box",           &MathMLElementFactory::createBoxElement },
    { "action",        &MathMLElementFactory::createBoxMLActionElement },
    { "obj",           &MathMLElementFactory::createBoxMLObjectElement },
#endif

    { "",              0 }
  };

  for (unsigned i = 0; tab[i].create; i++)
    factoryMethodMap[tab[i].tag] = tab[i].create;
}

MathMLElementFactory::~MathMLElementFactory()
{ }

SmartPtr<Element>
MathMLElementFactory::createElement(const String& name) const
{
  FactoryMethodMap::const_iterator p = factoryMethodMap.find(name);
  if (p != factoryMethodMap.end())
    return (this->*((*p).second))();
  else
    return createDummyElement();
}

