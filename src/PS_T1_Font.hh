// Copyright (C) 2001, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#ifndef PS_T1_Font_hh
#define PS_T1_Font_hh

#ifdef HAVE_LIBT1

#include "T1_Font.hh"

class PS_T1_Font : public T1_Font {
public:
  PS_T1_Font(unsigned, float);
  virtual ~PS_T1_Font();

  void ResetUsedChars(void) const;
  void UseChar(char ch) const { UseChars(&ch, 1); }
  void UseChars(const char*, unsigned) const;
  const char* GetUsedChars(void) const { return used; }

private:
  // the following array is used to mark chars which are actually used in the font,
  // in order to optimize the exported PostScript
  mutable char used[256];
};

#define TO_PS_T1_FONT(font) (dynamic_cast<const PS_T1_Font*>(font))

#endif // HAVE_LIBT1

#endif // PS_T1_Font_hh
