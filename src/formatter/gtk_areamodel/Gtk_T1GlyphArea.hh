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

#ifndef __Gtk_T1GlyphArea_hh__
#define __Gtk_T1GlyphArea_hh__

#include "GlyphArea.hh"

class Gtk_T1GlyphArea : public GlyphArea
{
protected:
  Gtk_T1GlyphArea(const SmartPtr<class T1Font>&, Char8);
  virtual ~Gtk_T1GlyphArea();

public:
  static SmartPtr<Gtk_T1GlyphArea> create(const SmartPtr<class T1Font>&, Char8);

  virtual BoundingBox box(void) const;
  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;
  virtual void render(class RenderingContext&, const scaled&, const scaled&) const;

  SmartPtr<class T1Font> getFont(void) const;
  Char8 getIndex(void) const { return index; }

private:
  SmartPtr<class T1Font> font;
  Char8 index;
};

#endif // __Gtk_T1GlyphArea_hh__
