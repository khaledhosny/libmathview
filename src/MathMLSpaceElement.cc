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

#include <cassert>

#include "defs.h"
#include "MathMLSpaceElement.hh"
#include "RenderingEnvironment.hh"
#include "AttributeParser.hh"
#include "FormattingContext.hh"
#include "ValueConversion.hh"

MathMLSpaceElement::MathMLSpaceElement(const SmartPtr<class MathMLView>& view)
  : MathMLElement(view)
{
  breakability = BREAK_AUTO;
}

MathMLSpaceElement::~MathMLSpaceElement()
{
}

void
MathMLSpaceElement::refine(AbstractRefinementContext& context)
{
  if (DirtyAttribute())
    {
      REFINE_ATTRIBUTE(context, Space, width);
      REFINE_ATTRIBUTE(context, Space, height);
      REFINE_ATTRIBUTE(context, Space, depth);
      REFINE_ATTRIBUTE(context, Space, linebreak);
      MathMLElement::refine(context);
    }
}

void
MathMLSpaceElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute())
    {
      background = env.GetBackgroundColor();

      scaled width;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Space, width))
	if (IsKeyword(value))
	  width = env.ToScaledPoints(ToNumberUnit(Resolve(value, env)));
	else
	  width = env.ToScaledPoints(ToNumberUnit(value));
      else
	assert(IMPOSSIBLE);

      scaled height;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Space, height))
	height = env.ToScaledPoints(ToNumberUnit(value));

      scaled depth;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Space, depth))
	depth = env.ToScaledPoints(ToNumberUnit(value));

      box.set(width, height, depth);

      if (!IsSet(ATTR_WIDTH) && !IsSet(ATTR_HEIGHT) && !IsSet(ATTR_DEPTH))
	breakability = ToBreakId(GET_ATTRIBUTE_VALUE(Space, linebreak));

      ResetDirtyAttribute();
    }
}

void
MathMLSpaceElement::DoLayout(const FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt)) ResetDirtyLayout(ctxt);
}

bool
MathMLSpaceElement::IsSpace() const
{
  return true;
}

bool
MathMLSpaceElement::IsSpaceLike() const
{
  return true;
}

scaled
MathMLSpaceElement::GetRightEdge() const
{
  return GetX() + (lineBreak ? box.width : 0);
}
