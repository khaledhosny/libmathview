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

#include "PS_BackgroundArea.hh"
#include "PS_RenderingContext.hh"

void
PS_BackgroundArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  PS_RenderingContext& context = dynamic_cast<PS_RenderingContext&>(c);
 
  const RGBColor old_foregroundColor = context.getForegroundColor();
  const RGBColor old_backgroundColor = context.getBackgroundColor();
  
  context.setForegroundColor(getColor());
  context.setBackgroundColor(getColor());
  context.fill(x, y, box());
  context.setForegroundColor(old_foregroundColor);
  getChild()->render(context, x, y);
  context.setBackgroundColor(old_backgroundColor);
}
