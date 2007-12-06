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

#include "Rope.hh"
#include <algorithm>

#include "BoxMLParagraphElement.hh"
#include "BoxMLVElement.hh"
#include "BoxMLAttributeSignatures.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "AreaFactory.hh"
#include "BoxMLTextElement.hh"
#include "BoxMLNamespaceContext.hh"
#include "BoxedParagraph.hh"

BoxMLParagraphElement::BoxMLParagraphElement(const SmartPtr<BoxMLNamespaceContext>& c)
  : BoxMLLinearContainerElement(c)
{ }

BoxMLParagraphElement::~BoxMLParagraphElement()
{ }

SmartPtr<BoxMLParagraphElement>
BoxMLParagraphElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLParagraphElement(context); }

AreaRef
BoxMLParagraphElement::format(FormattingContext& ctxt)
{
  assert(false);
  std::cerr << "sto per formattare il par " << dirtyLayout() << std::endl;

  if (dirtyLayout())
    {
      ctxt.push(this);

      const scaled availableWidth = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, Text, width)), 0);
      const scaled minLineSpacing = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, V, minlinespacing)), 0);

      scaled width;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(BoxML, Text, width))
	if (IsTokenId(value))
	  width = scaled::min();
	else
	  width = ctxt.BGD()->evaluate(ctxt, ToLength(value), scaled::max());
      else
	width = scaled::min();

      BoxedParagraph paragraph;
      for (std::vector< SmartPtr<BoxMLElement> >::const_iterator p = content.begin();
	   p != content.end();
	   p++)
	if (*p)
	  if (SmartPtr<BoxMLTextElement> text = smart_cast<BoxMLTextElement>(*p))
	    paragraph.append(ctxt, text->getContent());
	  else
	    paragraph.append(ctxt, (*p)->format(ctxt));

      AreaRef res = ctxt.BGD()->paragraph(ctxt, paragraph, width);
      setArea(ctxt.BGD()->wrapper(ctxt, res));

      ctxt.pop();
      resetDirtyLayout();
    }

  std::cerr << "fine formattazione par " << dirtyLayout() << std::endl;

  return getArea();
}
