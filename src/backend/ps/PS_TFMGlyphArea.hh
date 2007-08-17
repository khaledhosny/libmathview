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
