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

#include "View.hh"
#include "BoxMLSpaceElement.hh"
#include "BoxMLAttributeSignatures.hh"
#include "BoxFormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "AreaFactory.hh"

BoxMLSpaceElement::BoxMLSpaceElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLElement(context)
{ }

BoxMLSpaceElement::~BoxMLSpaceElement()
{ }

SmartPtr<BoxMLSpaceElement>
BoxMLSpaceElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLSpaceElement(context); }

AreaRef
BoxMLSpaceElement::makeSpaceArea(BoxFormattingContext& ctxt)
{
  scaled width = ctxt.getDevice()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, Space, width)), 0);
  scaled height = ctxt.getDevice()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, Space, height)), 0);
  scaled depth = ctxt.getDevice()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, Space, depth)), 0);

  std::vector<AreaRef> c;
  c.reserve(2);
  c.push_back(ctxt.getDevice()->getFactory()->verticalSpace(height, depth));
  c.push_back(ctxt.getDevice()->getFactory()->horizontalSpace(width));
  return ctxt.getDevice()->getFactory()->horizontalArray(c);
}

AreaRef
BoxMLSpaceElement::format(BoxFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      AreaRef res = makeSpaceArea(ctxt);
      res = ctxt.getDevice()->wrapper(ctxt, res);
      setArea(res);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
