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

#include <cassert>

#include "TFM.hh"
#include "TFMFont.hh"

TFMFont::TFMFont(const SmartPtr<TFM>& _tfm, const scaled& s)
  : T1Font(s), tfm(_tfm)
{
  assert(tfm);
}

TFMFont::~TFMFont()
{ }

SmartPtr<TFMFont>
TFMFont::create(const SmartPtr<TFM>& _tfm, const scaled& s)
{ return new TFMFont(_tfm, s); }

SmartPtr<TFM>
TFMFont::getTFM() const
{ return tfm; }

scaled
TFMFont::getGlyphLeftEdge(Char8) const
{ return 0; }

scaled
TFMFont::getGlyphRightEdge(Char8 index) const
{ return tfm->getGlyphBoundingBox(index).width * tfm->getScale(getSize()); }

BoundingBox
TFMFont::getGlyphBoundingBox(Char8 index) const
{
  const float scale = tfm->getScale(getSize());
  BoundingBox box = tfm->getGlyphBoundingBox(index);
  box.width *= scale;
  box.height *= scale;
  box.depth *= scale;
  return box;
}
