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

#include <cassert>

#include "mathVariantAux.hh"

static MathVariantAttributes vattr[] = {
  { T_NORMAL,            "serif",         T_NORMAL,    T_NORMAL },
  { T_BOLD,              "serif",         T_BOLD,      T_NORMAL },
  { T_ITALIC,            "serif",         T_NORMAL,    T_ITALIC },
  { T_BOLD_ITALIC,       "serif",         T_BOLD,      T_ITALIC },
  { T_DOUBLE_STRUCK,     "double-struck", T_NORMAL,    T_NORMAL },
  { T_BOLD_FRAKTUR,      "fraktur",       T_BOLD,      T_NORMAL },
  { T_SCRIPT,            "script",        T_NORMAL,    T_NORMAL },
  { T_BOLD_SCRIPT,       "script",        T_BOLD,      T_NORMAL },
  { T_FRAKTUR,           "fraktur",       T_NORMAL,    T_NORMAL },
  { T_SANS_SERIF,        "sans-serif",    T_NORMAL,    T_NORMAL },
  { T_BOLD_SANS_SERIF,   "sans-serif",    T_BOLD,      T_NORMAL },
  { T_SANS_SERIF_ITALIC, "sans-serif",    T_NORMAL,    T_ITALIC },
  { T_MONOSPACE,         "monospace",     T_NORMAL,    T_NORMAL },
  
  { T__NOTVALID,         0,               T__NOTVALID, T__NOTVALID }
};

const MathVariantAttributes&
attributesOfVariant(TokenId id)
{
  unsigned i;
  for (i = 0; vattr[i].kw != T__NOTVALID && vattr[i].kw != id; i++) ;
  assert(vattr[i].kw != T__NOTVALID);
  return vattr[i];
}
