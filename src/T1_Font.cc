// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#ifdef HAVE_LIBT1

#include <assert.h>
#include <t1lib.h>

#include "T1_Font.hh"
#include "T1_Gtk_DrawingArea.hh"

unsigned T1_Font::nextId = 0;

T1_Font::T1_Font(unsigned id, float s)
{
  fontId = nextId++;
  nativeFontId = id;
  scale = s;
}

T1_Font::~T1_Font()
{
}

scaled
T1_Font::CharWidth(char ch) const
{
  int nw = T1_GetCharWidth(nativeFontId, ch);
  return pt2sp(scale * nw / 1000.0);
}

scaled
T1_Font::CharHeight(char ch) const
{
  BBox charBox = T1_GetCharBBox(nativeFontId, ch);
  return pt2sp(scale * (charBox.ury - charBox.lly) / 1000.0);
}

scaled
T1_Font::GetAscent() const
{
  BBox fontBox = T1_GetFontBBox(nativeFontId);
  return pt2sp(scale * fontBox.ury / 1000.0);
}

scaled
T1_Font::GetDescent() const
{
  BBox fontBox = T1_GetFontBBox(nativeFontId);
  return pt2sp(scale * (-fontBox.lly) / 1000.0);
}

float
T1_Font::GetItalicAngle() const
{
  return T1_GetItalicAngle(nativeFontId);
}

scaled
T1_Font::GetEm() const
{
  return CharWidth('M');
}

scaled
T1_Font::GetEx() const
{
  return CharHeight('x');
}

scaled
T1_Font::GetAxis() const
{
  return GetEx() / 2;
}

scaled
T1_Font::GetKerning(char ch1, char ch2) const
{
  return pt2sp(scale * T1_GetKerning(nativeFontId, ch1, ch2) / 1000.0);
}

scaled
T1_Font::GetLineThickness() const
{
  // don't know exactly where the following heuristic comes from
  return float2sp(scale * 2500);
}

void
T1_Font::GetBoundingBox(BoundingBox& box) const
{
  BBox fontBox = T1_GetFontBBox(nativeFontId);
  box.Set(pt2sp(scale * (fontBox.urx - fontBox.llx) / 1000.0),
	  pt2sp(scale * fontBox.ury / 1000.0),
	  pt2sp(scale * (-fontBox.lly) / 1000.0));
}

void
T1_Font::CharBox(char ch, BoundingBox& box) const
{
  BBox charBox = T1_GetCharBBox(nativeFontId, ch);
  box.Set(CharWidth(ch),
	  pt2sp(scale * charBox.ury / 1000.0),
	  pt2sp(scale * (-charBox.lly) / 1000.0),
	  pt2sp(scale * charBox.llx / 1000.0),
	  pt2sp(scale * charBox.urx / 1000.0));
}

void
T1_Font::StringBox(const char* s, unsigned len, BoundingBox& box) const
{
  int sw = T1_GetStringWidth(nativeFontId, const_cast<char*>(s), len, 0, 0);
  BBox stringBox = T1_GetStringBBox(nativeFontId, const_cast<char*>(s), len, 0, 0);
  box.Set(pt2sp(scale * sw / 1000.0),
	  pt2sp(scale * stringBox.ury / 1000.0),
       	  pt2sp(scale * (-stringBox.lly) / 1000.0),
	  pt2sp(scale * stringBox.llx / 1000.0),
	  pt2sp(scale * stringBox.urx / 1000.0));
}

#endif // HAVE_LIBT1
