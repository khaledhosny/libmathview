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

#include <config.h>

#include <cassert>

#include "TFM.hh"
#include "t1lib_TFM_T1Font.hh"

t1lib_TFM_T1Font::t1lib_TFM_T1Font(int fid, const scaled& s, const SmartPtr<TFM>& _tfm)
  : t1lib_T1Font(fid, s), tfm(_tfm)
{
  assert(tfm);
}

t1lib_TFM_T1Font::~t1lib_TFM_T1Font()
{ }

SmartPtr<t1lib_TFM_T1Font>
t1lib_TFM_T1Font::create(int fid, const scaled& s, const SmartPtr<TFM>& _tfm)
{ return new t1lib_TFM_T1Font(fid, s, _tfm); }

SmartPtr<TFM>
t1lib_TFM_T1Font::getTFM() const
{ return tfm; }

BoundingBox
t1lib_TFM_T1Font::getGlyphBoundingBox(Char8 index) const
{
  const float scale = tfm->getScale(getSize());
  BoundingBox box = tfm->getGlyphBoundingBox(index);
  box.width *= scale;
  box.height *= scale;
  box.depth *= scale;
  return box;
}
