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

#include "String.hh"
#include "Iterator.hh"
#include "CharMapper.hh"
#include "DrawingArea.hh"
#include "FontAttributes.hh"
#include "FontifiedString.hh"

FontifiedString::FontifiedString(const String& s,
				 const FontAttributes& fa,
				 const CharMapper& cm)
{
  unsigned i = 0;
  while (i < s.GetLength()) {
    FontifiedChar fch;

    bool res = cm.FontifyChar(fch, fa, s.GetChar(i));
    assert(fch.font != NULL);
    assert(fch.charMap != NULL);

    Chunk* chunk = new Chunk;

    if (res) {
      unsigned start = i;
      
      do i++; while (i < s.GetLength() && fch.charMap->MapsChar(s.GetChar(i)));
    
      chunk->length = i - start;
      chunk->font   = fch.font;
      chunk->data   = new char[chunk->length];
    
      for (unsigned j = 0; j < chunk->length; j++)
	chunk->data[j] = fch.charMap->Map(s.GetChar(start + j));
    } else {
      i++;

      chunk->length = 1;
      chunk->font   = fch.font;
      chunk->data   = new char;
      chunk->data[0] = fch.nch;
    }
    
    content.Append(chunk);
  }
}

FontifiedString::~FontifiedString()
{
  for (Iterator<Chunk*> p(content); p.More(); p.Next()) {
    assert(p() != NULL);
    delete p();
  }
}

void
FontifiedString::GetBoundingBox(BoundingBox& box) const
{
  box.Null();

  for (Iterator<Chunk*> p(content); p.More(); p.Next()) {
    assert(p() != NULL);
    assert(p()->font != NULL);

    BoundingBox chunkBox;
    p()->font->StringBox(p()->data, p()->length, chunkBox);

    box.Append(chunkBox);
  }
}

scaled
FontifiedString::WidthTo(unsigned i) const
{
  BoundingBox box;
  box.Null();

  for (Iterator<Chunk*> p(content); p.More(); p.Next()) {
    assert(p() != NULL);
    assert(p()->font != NULL);

    bool last = (i <= p()->length);

    BoundingBox chunkBox;
    p()->font->StringBox(p()->data, last ? i : p()->length, chunkBox);

    box.Append(chunkBox);

    if (last) return box.rBearing;
    else i -= p()->length;
  }

  assert(IMPOSSIBLE);
  return 0;
}

void
FontifiedString::Draw(scaled x, scaled y, const DrawingArea& area, const GraphicsContext* gc) const
{
  for (Iterator<Chunk*> p(content); p.More(); p.Next()) {
    assert(p() != NULL);

    area.DrawString(gc, p()->font, x, y, p()->data, p()->length);
    x += p()->font->StringWidth(p()->data, p()->length);
  }
}
