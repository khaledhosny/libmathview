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

#ifndef __TFMFont_hh__
#define __TFMFont_hh__

#include "Char.hh"
#include "T1Font.hh"
#include "SmartPtr.hh"
#include "BoundingBox.hh"

class GMV_MathView_EXPORT TFMFont : public T1Font
{
protected:
  TFMFont(const SmartPtr<class TFM>&, const scaled&);
  virtual ~TFMFont();

public:
  static SmartPtr<TFMFont> create(const SmartPtr<class TFM>&, const scaled&);

  SmartPtr<class TFM> getTFM(void) const;
  
  virtual scaled getGlyphLeftEdge(Char8) const;
  virtual scaled getGlyphRightEdge(Char8) const;
  virtual BoundingBox getGlyphBoundingBox(Char8) const;

private:
  SmartPtr<class TFM> tfm;
};

#endif // __TFMFont_hh__
