// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#include <cassert>

#include "MathVariant.hh"
#include "MathVariantMap.hh"

#define MAP_NAME(n) map_variant_##n
#define DECLARE_MAP(n) DOM::Char32 MAP_NAME(n)(DOM::Char32);

namespace DOM = GdomeSmartDOM;

typedef DOM::Char32 (*mapType)(DOM::Char32);

DECLARE_MAP(bold)
DECLARE_MAP(italic)
DECLARE_MAP(bold_italic)
DECLARE_MAP(double_struck)
DECLARE_MAP(bold_fraktur)
DECLARE_MAP(script)
DECLARE_MAP(bold_script)
DECLARE_MAP(fraktur)
DECLARE_MAP(sans_serif)
DECLARE_MAP(bold_sans_serif)
DECLARE_MAP(sans_serif_italic)
DECLARE_MAP(sans_serif_bold_italic)
DECLARE_MAP(monospace)

static DOM::Char32 map_variant_normal(DOM::Char32 ch)
{ return ch; }

static mapType map[] =
{
  MAP_NAME(normal),
  MAP_NAME(bold),
  MAP_NAME(italic),
  MAP_NAME(bold_italic),
  MAP_NAME(double_struck),
  MAP_NAME(bold_fraktur),
  MAP_NAME(script),
  MAP_NAME(bold_script),
  MAP_NAME(fraktur),
  MAP_NAME(sans_serif),
  MAP_NAME(bold_sans_serif),
  MAP_NAME(sans_serif_italic),
  MAP_NAME(sans_serif_bold_italic),
  MAP_NAME(monospace)
};

DOM::Char32
mapMathVariant(MathVariant variant, DOM::Char32 ch)
{
  assert(variant >= NORMAL_VARIANT && variant <= MONOSPACE_VARIANT);
  return map[variant - NORMAL_VARIANT](ch);
}

void
mapMathVariant(MathVariant variant, DOM::UCS4String& str)
{
  assert(variant >= NORMAL_VARIANT && variant <= MONOSPACE_VARIANT);
  mapType m = map[variant - NORMAL_VARIANT];

  for (DOM::UCS4String::iterator p = str.begin();
       p != str.end(); p++)
    *p = m(*p);
}
