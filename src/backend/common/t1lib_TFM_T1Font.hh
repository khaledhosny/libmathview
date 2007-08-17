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

#ifndef __t1lib_TFM_T1Font_hh__
#define __t1lib_TFM_T1Font_hh__

#include "Char.hh"
#include "t1lib_T1Font.hh"
#include "SmartPtr.hh"
#include "BoundingBox.hh"

class GMV_MathView_EXPORT t1lib_TFM_T1Font : public t1lib_T1Font
{
protected:
  t1lib_TFM_T1Font(int, const scaled&, const SmartPtr<class TFM>&);
  virtual ~t1lib_TFM_T1Font();

public:
  static SmartPtr<t1lib_TFM_T1Font> create(int, const scaled&, const SmartPtr<class TFM>&);

  SmartPtr<class TFM> getTFM(void) const;

  virtual BoundingBox getGlyphBoundingBox(Char8) const;

private:
  SmartPtr<class TFM> tfm;
};

#endif // __t1lib_TFM_T1Font_hh__
