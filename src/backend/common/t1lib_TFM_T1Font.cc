// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

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
