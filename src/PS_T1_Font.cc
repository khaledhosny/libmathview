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

#include <config.h>

#ifdef HAVE_LIBT1

#include <assert.h>

#include "PS_T1_Font.hh"
#include "MathEngine.hh"

PS_T1_Font::PS_T1_Font(unsigned id, float s) : T1_Font(id, s)
{
  ResetUsedChars();
}

PS_T1_Font::~PS_T1_Font()
{
}

void
PS_T1_Font::UseChars(const char* s, unsigned length) const
{
  assert(s != NULL);
  for (unsigned i = 0; i < length; i++) {
    assert(((unsigned char) s[i]) < 256);
    used[((unsigned char) s[i])] = 1;
  }
}

void
PS_T1_Font::ResetUsedChars() const
{
  for (unsigned i = 0; i < 256; i++) used[i] = 0;
}

#endif // HAVE_LIBT1
