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
#include "BoxMLAttributeSignatures.hh"
#include "BoxMLVElement.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "ValueConversion.hh"

BoxMLVElement::BoxMLVElement(const SmartPtr<View>& view)
  : BoxMLLinearContainerElement(view)
{
}

BoxMLVElement::~BoxMLVElement()
{
}

SmartPtr<BoxMLVElement>
BoxMLVElement::create(const SmartPtr<View>& view)
{
  return new BoxMLVElement(view);
}

void
BoxMLVElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, BoxML, V, align);
      REFINE_ATTRIBUTE(context, BoxML, V, enter);
      REFINE_ATTRIBUTE(context, BoxML, V, exit);
      REFINE_ATTRIBUTE(context, BoxML, V, minlinespacing);
      BoxMLLinearContainerElement::refine(context);
    }
}

AreaRef
BoxMLVElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      TokenId align = ToTokenId(GET_ATTRIBUTE_VALUE(BoxML, V, align));
      int enter = ToInteger(GET_ATTRIBUTE_VALUE(BoxML, V, enter));
      int exit = ToInteger(GET_ATTRIBUTE_VALUE(BoxML, V, exit));
      scaled minLineSpacing = ctxt.getDevice()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, V, minlinespacing)), 0);

      if (enter < 0) enter = content.getSize() + enter + 1;
      if (exit < 0) exit = content.getSize() + exit + 1;

      enter = content.getSize() - std::min(std::max(enter, 1), (int) content.getSize());
      exit = content.getSize() - std::min(std::max(exit, 1), (int) content.getSize());

      int enter_index = 0;
      int exit_index = 0;
      AreaRef prevArea = 0;
      scaled prevHeight;
      std::vector<AreaRef> c;
      c.reserve(content.getSize());
      for (std::vector< SmartPtr<BoxMLElement> >::const_reverse_iterator p = content.rbegin();
	   p != content.rend();
	   p++)
	if (*p)
	  {
	    AreaRef area = (*p)->format(ctxt);
	    switch (align)
	      {
	      case T_LEFT: area = ctxt.getDevice()->getFactory()->left(area); break;
	      case T_CENTER: area = ctxt.getDevice()->getFactory()->center(area); break;
	      case T_RIGHT: area = ctxt.getDevice()->getFactory()->right(area); break;
	      default:
		assert(false);
		break;
	      }

	    BoundingBox areaBox = area->box();
	    if (prevArea)
	      {
		if (prevHeight + areaBox.depth < minLineSpacing)
		  c.push_back(ctxt.getDevice()->getFactory()->verticalSpace(minLineSpacing - prevHeight - areaBox.depth, 0));
	      }
	    prevHeight = areaBox.height;
	    prevArea = area;

	    if (enter-- == 0) enter_index = c.size();
	    if (exit-- == 0) exit_index = c.size();
	    c.push_back(area);
	  }

      AreaRef res = ctxt.getDevice()->getFactory()->verticalArray(c, enter_index);
      if (enter != exit)
	{
	  AreaRef res1 = ctxt.getDevice()->getFactory()->verticalArray(c, exit_index);
	  step = res->box().height - res1->box().height;
	}
      else
	step = 0;
      res = ctxt.getDevice()->wrapper(ctxt, res);
      setArea(res);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
