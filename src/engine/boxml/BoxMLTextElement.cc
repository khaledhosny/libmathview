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

#include "BoxMLAttributeSignatures.hh"
#include "BoxMLTextElement.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "AreaFactory.hh"

BoxMLTextElement::BoxMLTextElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLElement(context)
{ }

BoxMLTextElement::~BoxMLTextElement()
{ }

SmartPtr<BoxMLTextElement>
BoxMLTextElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLTextElement(context); }

AreaRef
BoxMLTextElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      RGBColor oldColor = ctxt.getColor();
      RGBColor oldBackground = ctxt.getBackground();

      ctxt.push(this);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(BoxML, Text, size))
	ctxt.setSize(ctxt.BGD()->evaluate(ctxt, ToLength(value), ctxt.getSize()));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(BoxML, Text, color))
	ctxt.setColor(ToRGB(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(BoxML, Text, background))
	ctxt.setBackground(ToRGB(value));

      scaled width;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(BoxML, Text, width))
	if (IsTokenId(value))
	  width = ctxt.getAvailableWidth();
	else
	  width = ctxt.BGD()->evaluate(ctxt, ToLength(value), scaled::max());
      else
	width = ctxt.getAvailableWidth();

      RGBColor newColor = ctxt.getColor();
      RGBColor newBackground = ctxt.getBackground();

      AreaRef res = ctxt.BGD()->string(ctxt, content, width);

      if (oldColor != newColor)
	res = ctxt.BGD()->getFactory()->color(res, newColor);

      if (!newBackground.transparent() && newBackground != oldBackground)
	res = ctxt.BGD()->getFactory()->background(res, newBackground);

      res = ctxt.BGD()->wrapper(ctxt, res);
      setMaxArea(res);
      setArea(res);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}

void
BoxMLTextElement::setContent(const String& s)
{
  if (s != content)
    {
      content = s;
      setDirtyLayout();
    }
}
