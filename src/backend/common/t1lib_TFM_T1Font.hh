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

#ifndef __t1lib_TFM_T1Font_hh__
#define __t1lib_TFM_T1Font_hh__

#include "Char.hh"
#include "t1lib_T1Font.hh"
#include "SmartPtr.hh"
#include "BoundingBox.hh"

class GMV_EXPORT t1lib_TFM_T1Font : public t1lib_T1Font
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
