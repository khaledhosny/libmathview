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

#include "BoxMLGroupElement.hh"
#include "FormattingContext.hh"
#include "BoxMLAttributeSignatures.hh"

BoxMLGroupElement::BoxMLGroupElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLLinearContainerElement(context)
{ }

BoxMLGroupElement::~BoxMLGroupElement()
{ }

SmartPtr<BoxMLGroupElement>
BoxMLGroupElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLGroupElement(context); }

AreaRef
BoxMLGroupElement::format(FormattingContext& ctxt)
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
BoxMLGroupElement::formatContent(FormattingContext& ctxt, std::vector<AreaRef>& areas)
{
  ctxt.push(this);
  for (std::vector< SmartPtr<BoxMLElement> >::const_iterator p = content.begin();
       p != content.end();
       p++)
    if (SmartPtr<BoxMLGroupElement> group = smart_cast<BoxMLGroupElement>(*p))
      group->formatContent(ctxt, areas);
    else
      areas.push_back((*p)->format(ctxt));
  ctxt.pop();
}
