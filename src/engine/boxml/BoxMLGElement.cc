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

#include "for_each_if.h"
#include "Adaptors.hh"
#include "BoxMLGElement.hh"
#include "BoxMLAttributeSignatures.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "ValueConversion.hh"
#include "BoxMLAtElement.hh"

BoxMLGElement::BoxMLGElement(const SmartPtr<View>& view)
  : BoxMLLinearContainerElement(view)
{ }

BoxMLGElement::~BoxMLGElement()
{ }

SmartPtr<BoxMLGElement>
BoxMLGElement::create(const SmartPtr<View>& view)
{
  return new BoxMLGElement(view);
}

void
BoxMLGElement::refine(class AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, BoxML, G, width);
      REFINE_ATTRIBUTE(context, BoxML, G, height);
      REFINE_ATTRIBUTE(context, BoxML, G, depth);
      BoxMLLinearContainerElement::refine(context);
    }
}

AreaRef
BoxMLGElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      scaled width = ctxt.getDevice()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, G, width)), ctxt.getSize());
      scaled height = ctxt.getDevice()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, G, height)), ctxt.getSize());
      scaled depth = ctxt.getDevice()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, G, depth)), ctxt.getSize());

      scaled step = 0;
      std::vector<BoxedLayoutArea::XYArea> c;
      c.reserve(content.getSize());
      for (std::vector< SmartPtr<BoxMLElement> >::const_iterator p = content.begin();
	   p != content.end();
	   p++)
	if (SmartPtr<BoxMLAtElement> elem = smart_cast<BoxMLAtElement>(*p))
	  {
	    AreaRef area = elem->format(ctxt);
	    c.push_back(BoxedLayoutArea::XYArea(elem->getX(), elem->getY(), area));
	  }

      AreaRef res = ctxt.getDevice()->getFactory()->boxedLayout(BoundingBox(width, height, depth), c);
      res = ctxt.getDevice()->wrapper(ctxt, res);
      setArea(res);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
