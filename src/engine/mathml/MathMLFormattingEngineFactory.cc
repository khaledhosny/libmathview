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

#include "MathMLFormattingEngineFactory.hh"

MathMLFormattingEngineFactory::MathMLFormattingEngineFactory()
{
  static struct
  {
    String tag;
    FactoryMethod create;
  } tab[] = {
    { "math",          &MathMLFormattingEngineFactory::createMathElement },
    { "mi",            &MathMLFormattingEngineFactory::createIdentifierElement },
    { "mn",            &MathMLFormattingEngineFactory::createNumberElement },
    { "mo",            &MathMLFormattingEngineFactory::createOperatorElement },
    { "mtext",         &MathMLFormattingEngineFactory::createTextElement },
    { "mspace",        &MathMLFormattingEngineFactory::createSpaceElement },
    { "ms",            &MathMLFormattingEngineFactory::createStringLitElement },
    { "mrow",          &MathMLFormattingEngineFactory::createRowElement },
    { "mfrac",         &MathMLFormattingEngineFactory::createFractionElement },
    { "msqrt",         &MathMLFormattingEngineFactory::createRadicalElement },
    { "mroot",         &MathMLFormattingEngineFactory::createRadicalElement },
    { "mstyle",        &MathMLFormattingEngineFactory::createStyleElement },
    { "merror",        &MathMLFormattingEngineFactory::createErrorElement },
    { "mpadded",       &MathMLFormattingEngineFactory::createPaddedElement },
    { "mphantom",      &MathMLFormattingEngineFactory::createPhantomElement },
    { "mfenced",       &MathMLFormattingEngineFactory::createFencedElement },
    { "msub",          &MathMLFormattingEngineFactory::createScriptElement },
    { "msup",          &MathMLFormattingEngineFactory::createScriptElement },
    { "msubsup",       &MathMLFormattingEngineFactory::createScriptElement },
    { "munder",        &MathMLFormattingEngineFactory::createUnderOverElement },
    { "mover",         &MathMLFormattingEngineFactory::createUnderOverElement },
    { "munderover",    &MathMLFormattingEngineFactory::createUnderOverElement },
    { "mmultiscripts", &MathMLFormattingEngineFactory::createMultiScriptsElement },
    { "mtable",        &MathMLFormattingEngineFactory::createTableElement },
    { "mtr",           &MathMLFormattingEngineFactory::createTableRowElement },
    { "mlabeledtr",    &MathMLFormattingEngineFactory::createLabeledTableRowElement },
    { "mtd",           &MathMLFormattingEngineFactory::createTableCellElement },
    { "maligngroup",   &MathMLFormattingEngineFactory::createAlignGroupElement },
    { "malignmark",    &MathMLFormattingEngineFactory::createAlignMarkElement },
    { "maction",       &MathMLFormattingEngineFactory::createActionElement },
    { "menclose",      &MathMLFormattingEngineFactory::createEncloseElement },
    { "semantics",     &MathMLFormattingEngineFactory::createSemanticsElement },

    // TEMPORARY FACTORY METHODS FOR BOXML
    { "at",            &MathMLFormattingEngineFactory::createAtElement },
    { "g",             &MathMLFormattingEngineFactory::createGElement },
    { "h",             &MathMLFormattingEngineFactory::createHElement },
    { "ink",           &MathMLFormattingEngineFactory::createInkElement },
    { "space",         &MathMLFormattingEngineFactory::createBoxMLSpaceElement },
    { "text",          &MathMLFormattingEngineFactory::createBoxMLTextElement },
    { "v",             &MathMLFormattingEngineFactory::createVElement },
    { "box",           &MathMLFormattingEngineFactory::createBoxElement },

    { "",              0 }
  };

  for (unsigned i = 0; tab[i].create; i++)
    factoryMethodMap[tab[i].tag] = tab[i].create;
}

MathMLFormattingEngineFactory::~MathMLFormattingEngineFactory()
{
}

SmartPtr<MathMLElement>
MathMLFormattingEngineFactory::createElement(const SmartPtr<class MathMLView>& view, const String& name) const
{
  FactoryMethodMap::const_iterator p = factoryMethodMap.find(name);
  if (p != factoryMethodMap.end())
    return (this->*((*p).second))(view);
  else
    return createDummyElement(view);
}

