// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include "BoxMLAttributeSignatures.hh"
#include "BoxMLHVElement.hh"
#include "BoxMLHElement.hh"
#include "BoxMLVElement.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "AreaFactory.hh"

BoxMLHVElement::BoxMLHVElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLLinearContainerElement(context)
{ }

BoxMLHVElement::~BoxMLHVElement()
{ }

SmartPtr<BoxMLHVElement>
BoxMLHVElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLHVElement(context); }

AreaRef
BoxMLHVElement::getMaxArea() const
{ return maxArea; }

AreaRef
BoxMLHVElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      const scaled spacing = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, HV, spacing)), 0);
      const scaled indent = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, HV, indent)), 0);
      const scaled minLineSpacing = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, V, minlinespacing)), 0);

      const scaled availableWidth = ctxt.getAvailableWidth();
      std::vector<AreaRef> c;
      c.reserve(content.getSize());
      std::vector<AreaRef> cMax;
      cMax.reserve(content.getSize());
      std::vector<scaled> sc;
      sc.reserve(content.getSize());

      for (std::vector< SmartPtr<BoxMLElement> >::const_iterator p = content.begin();
	   p != content.end();
	   p++)
	if (*p)
	  {
	    const scaled thisIndent = (p == content.begin()) ? 0 : indent;
	    ctxt.setAvailableWidth(availableWidth - thisIndent);
	    c.push_back((*p)->format(ctxt));
	    cMax.push_back((*p)->getMaxArea());

	    if (p + 1 != content.end())
	      sc.push_back(spacing);
	  }

      AreaRef res;
      res = BoxMLHElement::formatHorizontalArray(ctxt, cMax, spacing);
      res = ctxt.BGD()->wrapper(ctxt, res);
      setMaxArea(res);
      
      if (res->box().width > availableWidth)
	{
	  res = BoxMLVElement::formatVerticalArray(ctxt, c, minLineSpacing, 1, -1, indent);
	  res = ctxt.BGD()->wrapper(ctxt, res);
	}

      setArea(res);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
