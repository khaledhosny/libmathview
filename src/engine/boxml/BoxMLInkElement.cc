// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

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

