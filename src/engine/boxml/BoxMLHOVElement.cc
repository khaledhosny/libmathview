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
#include "BoxMLHOVElement.hh"
#include "BoxMLHElement.hh"
#include "BoxMLVElement.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "AreaFactory.hh"

#if 0
TODO

1) distribuire ampiezza residua in elemento H
2) fissare bug di allineamento hov (step?) e forse anche in hv
#endif

BoxMLHOVElement::BoxMLHOVElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLLinearContainerElement(context)
{ }

BoxMLHOVElement::~BoxMLHOVElement()
{ }

SmartPtr<BoxMLHOVElement>
BoxMLHOVElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLHOVElement(context); }

AreaRef
BoxMLHOVElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      const SmartPtr<Value> spacingV = GET_ATTRIBUTE_VALUE(BoxML, HOV, spacing);
      const SmartPtr<Value> indentV = GET_ATTRIBUTE_VALUE(BoxML, HOV, indent);
      const scaled minLineSpacing = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, V, minlinespacing)), 0);

      const scaled availableWidth = ctxt.getAvailableWidth();
      std::vector<SmartPtr<BoxMLElement> > hc;
      hc.reserve(content.getSize());
      std::vector<AreaRef> vc;
      vc.reserve(content.getSize());
      std::vector<SmartPtr<Value> > ic;
      ic.reserve(content.getSize());
      std::vector<scaled> sc;
      sc.reserve(content.getSize());
      
      std::vector< SmartPtr<BoxMLElement> >::const_iterator p = content.begin();
      while (p != content.end())
	{
	  hc.clear();
	  sc.clear();

	  const SmartPtr<Value> indent = GetComponent(indentV, p - content.begin());
	  const scaled remainingWidthForThisLine = availableWidth - BoxMLVElement::getMinimumIndentation(ctxt, indent);
	  ctxt.setAvailableWidth(remainingWidthForThisLine);
	  AreaRef firstArea = (*p)->format(ctxt);
	  ic.push_back(indent);
	  hc.push_back(*p);
	  scaled remainingWidth = remainingWidthForThisLine - firstArea->box().width;
	  p++;

	  while (p != content.end())
	    {
	      const scaled spacing = ctxt.BGD()->evaluate(ctxt, ToLength(GetComponent(spacingV, (p - 1) - content.begin())), scaled::zero());
	      remainingWidth -= spacing;
	      const SmartPtr<Value> indent = GetComponent(indentV, p - content.begin());
	      const scaled width = std::max(remainingWidth, availableWidth - BoxMLVElement::getMinimumIndentation(ctxt, indent));
	      ctxt.setAvailableWidth(width);
	      AreaRef area = (*p)->format(ctxt);
	      const scaled areaWidth = area->box().width;
	      if (areaWidth > remainingWidth)
		break;

	      remainingWidth -= areaWidth;
	      sc.push_back(spacing);
	      hc.push_back(*p);
	      p++;
	    }

	  vc.push_back(BoxMLHElement::formatHorizontalArray(ctxt, hc, sc, step));
	}

      AreaRef res = BoxMLVElement::formatVerticalArray(ctxt, vc, minLineSpacing, 1, vc.size(), ic, step);
      res = ctxt.BGD()->wrapper(ctxt, res);
      setArea(res);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
