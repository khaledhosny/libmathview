// Copyright (C) 2000-2002, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include <cassert>

#include "AreaFactory.hh"
#include "MathMLEmbellishment.hh"
#include "MathMLOperatorElement.hh"
#include "MathGraphicDevice.hh"
#include "MathFormattingContext.hh"

AreaRef
MathMLEmbellishment::formatEmbellishment(const SmartPtr<MathMLElement>& elem,
					 const MathFormattingContext& context,
					 const AreaRef& area) const
{
  assert(elem);
  if (SmartPtr<MathMLOperatorElement> top = elem->getCoreOperatorTop())
    {
      // left/right paddings are calculated during formatting, so these
      // quantities should be requested only AFTER that the operator has
      // been formatted. Checking for top->dirtyLayout() is not
      // quite right, though, in the case the operator itself is the
      // top core operator (see MathMLOperatorElement), because the
      // dirtyLayout flag is reset only after the formatEmbellishment
      // method is called
      // assert(!top->dirtyLayout());
      scaled leftPadding = top->getLeftPadding();
      scaled rightPadding = top->getRightPadding();
      if (leftPadding != scaled::zero() || rightPadding != scaled::zero())
	{
	  std::vector<AreaRef> row;
	  row.reserve(3);
	  row.push_back(context.getDevice()->getFactory()->horizontalSpace(leftPadding));
	  row.push_back(area);
	  row.push_back(context.getDevice()->getFactory()->horizontalSpace(rightPadding));
	  return context.getDevice()->getFactory()->horizontalArray(row);
	}
      else
	return area;
    }
  else
    return area;
}

