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

#include "BoxMLGroupElement.hh"

BoxMLGroupElement::BoxMLGroupElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLLinearContainerElement(context)
{ }

BoxMLGroupElement::~BoxMLGroupElement()
{ }

void
BoxMLGroupElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, BoxML, Group, size);
      REFINE_ATTRIBUTE(context, BoxML, Group, color);
      REFINE_ATTRIBUTE(context, BoxML, Group, background);
      BoxMLLinearContainerElement::refine(context);
    }
}

AreaRef
BoxMLGroupElement::format(BoxFormattingContext& context)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}

void
BoxMLGroupElement::formatContent(BoxFormattingContext& ctxt, std::vector<AreaRef>& areas)
{
  ctxt.push(this);
  for (std::vector< SmartPtr<BoxMLElement> >::const_iterator p = content.begin();
       p != content.end();
       p++)
    if (SmartPtr<BoxMLGroupElement> group = smart_cast<BoxMLGroupElement>(*p))
      group->formatContent(ctxt, areas);
    else
      areas.push_back((*p)->format(context));
  ctxt.pop();
}
