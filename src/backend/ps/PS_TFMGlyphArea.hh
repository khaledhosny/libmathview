// Copyright (C) 2000-2006, Luca Padovani <padovani@sti.uniurb.it>.
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
// <padovani@sti.uniurb.it>

#ifndef __PS_TFMGlyphArea_hh__
#define __PS_TFMGlyphArea_hh__

#include "GlyphArea.hh"

class PS_TFMGlyphArea : public GlyphArea
{
protected:
  PS_TFMGlyphArea(const SmartPtr<class TFMFont>&, Char8);
  virtual ~PS_TFMGlyphArea();

public:
  static SmartPtr<PS_TFMGlyphArea> create(const SmartPtr<class TFMFont>&, Char8);

  virtual BoundingBox box(void) const;
  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;
  virtual void render(class RenderingContext&, const scaled&, const scaled&) const;

  SmartPtr<class TFMFont> getFont(void) const;
  Char8 getIndex(void) const { return index; }

private:
  SmartPtr<class TFMFont> font;
  Char8 index;
};

#endif // __PS_TFMGlyphArea_hh__
