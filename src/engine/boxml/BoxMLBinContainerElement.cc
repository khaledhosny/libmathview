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

#include "defs.h"
#include "ChildList.hh"
#include "BoxMLBinContainerElement.hh"

BoxMLBinContainerElement::BoxMLBinContainerElement(const SmartPtr<View>& view)
  : BoxMLElement(view)
{
}

BoxMLBinContainerElement::~BoxMLBinContainerElement()
{
}

void
BoxMLBinContainerElement::construct()
{
  if (dirtyStructure())
    {
      // editing is supported with DOM only
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  ChildList children(getDOMElement(), BOXML_NS_URI, "*");
	  if (DOM::Node node = children.item(0))
	    if (SmartPtr<BoxMLElement> elem = smart_cast<BoxMLElement>(getFormattingNode(node)))
	      setChild(elem);
	}
#endif // HAVE_GMETADOM

      if (SmartPtr<BoxMLElement> child = getChild())
	child->construct();

      resetDirtyStructure();
    }
}

void
BoxMLBinContainerElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      if (SmartPtr<BoxMLElement> child = getChild())
	child->refine(context);
      resetDirtyAttribute();
    }
}

AreaRef
BoxMLBinContainerElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      if (SmartPtr<BoxMLElement> child = getChild())
	setArea(child->format(ctxt));
      resetDirtyAttribute();
    }

  return getArea();
}

scaled
BoxMLBinContainerElement::getStep() const
{
  if (SmartPtr<BoxMLElement> child = getChild())
    return child->getStep();
  else
    return scaled::zero();
}
