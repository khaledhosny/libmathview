// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#include <config.h>

#include "Rope.hh"
#include <algorithm>

#include "BoxMLParagraphElement.hh"
#include "BoxMLVElement.hh"
#include "BoxMLAttributeSignatures.hh"
#include "BoxFormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "AreaFactory.hh"
#include "BoxMLTextElement.hh"
#include "BoxMLNamespaceContext.hh"
#include "BoxMLElementFactory.hh"

BoxMLParagraphElement::BoxMLParagraphElement(const SmartPtr<BoxMLNamespaceContext>& c)
  : BoxMLLinearContainerElement(c)
{ }

BoxMLParagraphElement::~BoxMLParagraphElement()
{ }

SmartPtr<BoxMLParagraphElement>
BoxMLParagraphElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLParagraphElement(context); }

void
BoxMLParagraphElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, BoxML, V, align);
      REFINE_ATTRIBUTE(context, BoxML, V, minlinespacing);
      REFINE_ATTRIBUTE(context, BoxML, Text, width);
      BoxMLLinearContainerElement::refine(context);
    }
}

AreaRef
BoxMLParagraphElement::format(BoxFormattingContext& ctxt)
{
  std::cerr << "sto per formattare il par " << dirtyLayout() << std::endl;

  if (dirtyLayout())
    {
      ctxt.push(this);

      const scaled availableWidth = ctxt.getDevice()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, Text, width)), 0);
      const scaled minLineSpacing = ctxt.getDevice()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, V, minlinespacing)), 0);

      scaled width;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(BoxML, Text, width))
	if (IsTokenId(value))
	  width = scaled::min();
	else
	  width = ctxt.getDevice()->evaluate(ctxt, ToLength(value), scaled::max());
      else
	width = scaled::min();

      const String objectReplacement = fromUCS4String(DOM::UCS4String(1, 0xfffc));
      std::vector< std::pair<unsigned, AreaRef> > boxes;
      ROPE_NS::rope<String::value_type> c;
      for (std::vector< SmartPtr<BoxMLElement> >::const_iterator p = content.begin();
	   p != content.end();
	   p++)
	if (*p)
	  {
	    if (p != content.begin()) c.append(' ');
	    if (SmartPtr<BoxMLTextElement> text = smart_cast<BoxMLTextElement>(*p))
	      {
		const String s = text->getContent();
		c.append(s.data(), s.length());
	      }
	    else
	      {
		boxes.push_back(std::make_pair(c.size(), (*p)->format(ctxt)));
		c.append(objectReplacement.data(), objectReplacement.length());
	      }
	  }

      AreaRef res = ctxt.getDevice()->paragraph(ctxt, String(c.begin(), c.end()), boxes, width);
      setArea(ctxt.getDevice()->wrapper(ctxt, res));

      ctxt.pop();
      resetDirtyLayout();
    }

  std::cerr << "fine formattazione par " << dirtyLayout() << std::endl;

  return getArea();
}