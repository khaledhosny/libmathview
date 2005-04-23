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
#include "BoxMLHElement.hh"
#include "BoxMLVElement.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "AreaFactory.hh"

BoxMLHElement::BoxMLHElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLLinearContainerElement(context)
{ }

BoxMLHElement::~BoxMLHElement()
{ }

SmartPtr<BoxMLHElement>
BoxMLHElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLHElement(context); }

AreaRef
BoxMLHElement::formatHorizontalArray(FormattingContext& ctxt,
				     const std::vector<SmartPtr<BoxMLElement> >& content,
				     const std::vector<scaled>& spacing,
				     scaled& step)
{
  assert(content.size() == spacing.size() + 1);

  step = 0;

  std::vector<AreaRef> c;
  c.reserve(content.size());
  for (std::vector< SmartPtr<BoxMLElement> >::const_iterator p = content.begin();
       p != content.end();
       p++)
    if (SmartPtr<BoxMLElement> el = *p)
      {
	AreaRef area = el->format(ctxt);
	assert(area);
	if (step != scaled::zero())
	  area = ctxt.BGD()->getFactory()->shift(area, step);
	step += el->getStep();
	
	c.push_back(area);

	if (p + 1 != content.end())
	  {
	    const scaled thisSpacing = spacing[p - content.begin()];
	    if (thisSpacing != scaled::zero())
	      c.push_back(ctxt.BGD()->getFactory()->horizontalSpace(thisSpacing));
	  }
      }

  AreaRef res;
  if (c.size() == 1)
    res = c[0];
  else
    res = ctxt.BGD()->getFactory()->horizontalArray(c);

  return res;
}

AreaRef
BoxMLHElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      const SmartPtr<Value> spacingV = GET_ATTRIBUTE_VALUE(BoxML, H, spacing);

      std::vector<scaled> sc;
      sc.reserve(content.getSize());
      for (unsigned i = 1; i < content.getSize(); i++)
	sc.push_back(ctxt.BGD()->evaluate(ctxt, ToLength(GetComponent(spacingV, i - 1)), scaled::zero()));

      AreaRef res = formatHorizontalArray(ctxt, content.getContent(), sc, step);
      res = ctxt.BGD()->wrapper(ctxt, res);
      setArea(res);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
