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

#include <assert.h>

#include "Gtk_Font.hh"
#include "UnitValue.hh"

Gtk_Font::Gtk_Font(GdkFont* native_font)
{
  assert(native_font != NULL);

  font = native_font;
  gdk_font_ref(font);
}

Gtk_Font::Gtk_Font(const Gtk_Font& f)
{
  font = f.font;
  gdk_font_ref(font);
}

Gtk_Font::~Gtk_Font()
{
  gdk_font_unref(font);
}

scaled
Gtk_Font::GetAscent() const
{
  return px2sp(font->ascent);
}

scaled
Gtk_Font::GetDescent() const
{
  return px2sp(font->descent);
}

float
Gtk_Font::GetItalicAngle() const
{
  // no italic angle information available for X fonts
  return 0.0;
}

scaled
Gtk_Font::CharWidth(char ch) const
{
  return px2sp(gdk_char_width(font, ch));
}

scaled
Gtk_Font::CharHeight(char ch) const
{
  return px2sp(gdk_char_height(font, ch));
}

scaled
Gtk_Font::GetEm() const
{
  return CharWidth('M');
}

scaled
Gtk_Font::GetEx() const
{
  return CharHeight('x');
}

scaled
Gtk_Font::GetAxis() const
{
  return GetEx() / 2;
}

scaled
Gtk_Font::GetKerning(char ch1, char ch2) const
{
  // little hope to have kerning information for X fonts
  return 0;
}

scaled
Gtk_Font::GetLineThickness() const
{
  return float2sp(sp2pt(GetAscent() + GetDescent()) * 2500);
}

void
Gtk_Font::GetBoundingBox(BoundingBox& box) const
{
  box.Set(GetEm(), GetAscent(), GetDescent());
}

void
Gtk_Font::StringBox(const char* s, unsigned length, BoundingBox& box) const
{
  assert(s != NULL);
  assert(font != NULL);

  gint width;
  gint lbearing, rbearing;
  gint ascent, descent;

  gdk_text_extents(font,
		   s, length,
		   &lbearing, &rbearing,
		   &width,
		   &ascent, &descent);

  box.Set(px2sp(width), px2sp(ascent), px2sp(descent), px2sp(lbearing), px2sp(rbearing));
}

void
Gtk_Font::CharBox(char ch, BoundingBox& box) const
{
  StringBox(&ch, 1, box);
}



