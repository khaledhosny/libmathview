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

#include "BoxMLAtElement.hh"
#include "BoxMLAttributeSignatures.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "ValueConversion.hh"

BoxMLAtElement::BoxMLAtElement(const SmartPtr<View>& view)
  : BoxMLBinContainerElement(view)
{
}

BoxMLAtElement::~BoxMLAtElement()
{
}

SmartPtr<BoxMLAtElement>
BoxMLAtElement::create(const SmartPtr<View>& view)
{
  return new BoxMLAtElement(view);
}

void
BoxMLAtElement::refine(class AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, BoxML, At, x);
      REFINE_ATTRIBUTE(context, BoxML, At, y);
      BoxMLBinContainerElement::refine(context);
    }
}

AreaRef
BoxMLAtElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      x = ctxt.getDevice()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, At, x)), ctxt.getSize());
      y = ctxt.getDevice()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, At, y)), ctxt.getSize());

      if (getChild())
	{
	  AreaRef res = getChild()->format(ctxt);
	  res = ctxt.getDevice()->wrapper(ctxt, res);
	  setArea(res);
	}
      else
	setArea(0);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
