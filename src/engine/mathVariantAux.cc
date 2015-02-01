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
#include <cstring>

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
  
  { T__NOTVALID,         nullptr,               T__NOTVALID, T__NOTVALID }
};

const MathVariantAttributes&
attributesOfVariant(TokenId id)
{
  unsigned i;
  for (i = 0; vattr[i].kw != T__NOTVALID && vattr[i].kw != id; i++) ;
  assert(vattr[i].kw != T__NOTVALID);
  return vattr[i];
}

static int
attributesMatch(const char* family, TokenId weight, TokenId style,
		const MathVariantAttributes* attributes)
{
  int v = 0;
  if (family && attributes->family) v = strcmp(family, attributes->family) ? v : v + 1;
  if (weight != T__NOTVALID) v = (weight == attributes->weight) ? v : v + 1;
  if (style != T__NOTVALID) v = (style == attributes->style) ? v : v + 1;
  return v;
}

TokenId
variantOfAttributes(const char* family, TokenId weight, TokenId style)
{
  int best = -1;
  int bestValue = 0;
  for (int i = 0; vattr[i].kw != T__NOTVALID; i++)
    {
      int v = attributesMatch(family, weight, style, vattr + i);
      if (v > bestValue) best = i;
    }

  return (best >= 0) ? vattr[best].kw : T__NOTVALID;
}
