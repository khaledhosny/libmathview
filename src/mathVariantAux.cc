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

#include <assert.h>

#include <stddef.h>

#include "mathVariantAux.hh"

static MathVariantAttributes vattr[] = {
  { KW_NORMAL,            "serif",         FONT_WEIGHT_NORMAL,   FONT_STYLE_NORMAL },
  { KW_BOLD,              "serif",         FONT_WEIGHT_BOLD,     FONT_STYLE_NORMAL },
  { KW_ITALIC,            "serif",         FONT_WEIGHT_NORMAL,   FONT_STYLE_ITALIC },
  { KW_BOLD_ITALIC,       "serif",         FONT_WEIGHT_BOLD,     FONT_STYLE_ITALIC },
  { KW_DOUBLE_STRUCK,     "double-struck", FONT_WEIGHT_NORMAL,   FONT_STYLE_NORMAL },
  { KW_BOLD_FRAKTUR,      "fraktur",       FONT_WEIGHT_BOLD,     FONT_STYLE_NORMAL },
  { KW_SCRIPT,            "script",        FONT_WEIGHT_NORMAL,   FONT_STYLE_NORMAL },
  { KW_BOLD_SCRIPT,       "script",        FONT_WEIGHT_BOLD,     FONT_STYLE_NORMAL },
  { KW_FRAKTUR,           "fraktur",       FONT_WEIGHT_NORMAL,   FONT_STYLE_NORMAL },
  { KW_SANS_SERIF,        "sans-serif",    FONT_WEIGHT_NORMAL,   FONT_STYLE_NORMAL },
  { KW_BOLD_SANS_SERIF,   "sans-serif",    FONT_WEIGHT_BOLD,     FONT_STYLE_NORMAL },
  { KW_SANS_SERIF_ITALIC, "sans-serif",    FONT_WEIGHT_NORMAL,   FONT_STYLE_ITALIC },
  { KW_MONOSPACE,         "monospace",     FONT_WEIGHT_NORMAL,   FONT_STYLE_NORMAL },
  
  { KW_NOTVALID,          NULL,            FONT_WEIGHT_NOTVALID, FONT_STYLE_NOTVALID }
};

const MathVariantAttributes&
attributesOfVariant(KeywordId id)
{
  unsigned i;
  for (i = 0; vattr[i].kw != KW_NOTVALID && vattr[i].kw != id; i++) ;
  assert(vattr[i].kw != KW_NOTVALID);
  return vattr[i];
}
