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
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"

BoxMLAtElement::BoxMLAtElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLBinContainerElement(context)
{ }

BoxMLAtElement::~BoxMLAtElement()
{ }

SmartPtr<BoxMLAtElement>
BoxMLAtElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLAtElement(context); }

AreaRef
BoxMLAtElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      x = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, At, x)), ctxt.getSize());
      y = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, At, y)), ctxt.getSize());

      if (getChild())
	{
	  AreaRef res = getChild()->format(ctxt);
	  res = ctxt.BGD()->wrapper(ctxt, res);
	  setArea(res);
	}
      else
	setArea(0);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
