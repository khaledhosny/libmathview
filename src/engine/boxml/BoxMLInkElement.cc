// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include "BoxMLInkElement.hh"
#include "BoxMLAttributeSignatures.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "AreaFactory.hh"

BoxMLInkElement::BoxMLInkElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLSpaceElement(context)
{ }

BoxMLInkElement::~BoxMLInkElement()
{ }

SmartPtr<BoxMLInkElement>
BoxMLInkElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLInkElement(context); }

AreaRef
BoxMLInkElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      RGBColor oldColor = ctxt.getColor();

      ctxt.push(this);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(BoxML, Ink, color))
	ctxt.setColor(ToRGB(value));

      RGBColor newColor = ctxt.getColor();

      AreaRef res = makeSpaceArea(ctxt);
      res = ctxt.BGD()->getFactory()->ink(res);
      if (oldColor != newColor) res = ctxt.BGD()->getFactory()->color(res, newColor);
      res = ctxt.BGD()->wrapper(ctxt, res);
      setMaxArea(res);
      setArea(res);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}

