// (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include "Gtk_WrapperArea.hh"
#include "Gtk_RenderingContext.hh"
#include "SearchingContext.hh"

Gtk_WrapperArea::Gtk_WrapperArea(const AreaRef& area, const BoundingBox& b, const SmartPtr<Object>& el)
  : BoxArea(area, b), element(el)
{ }

void
Gtk_WrapperArea::render(RenderingContext& context, const scaled& x, const scaled& y) const
{
  Gtk_RenderingContext& c = dynamic_cast<Gtk_RenderingContext&>(context);

  // ...
  getChild()->render(context, x, y);
  // ...
}

#include "scaledAux.hh"
#include "BoundingBoxAux.hh"

bool
Gtk_WrapperArea::find(SearchingContext& context, const scaled& x, const scaled& y) const
{
  if (context.accept(this, x, y))
    {
#if 0
  std::cerr << "Wrapper::find: " << box() << " (" << x << "," << y << ") "
	    << " SEARCHING FOR (" << context.getX() << "," << context.getY() << ")" << std::endl;
#endif


      if (!BoxArea::find(context, x, y))
	context.setResult(this);
      return true;
    }
  else
    return BoxArea::find(context, x, y);
}

SmartPtr<Object>
Gtk_WrapperArea::getElement(void) const
{
  return element;
}
