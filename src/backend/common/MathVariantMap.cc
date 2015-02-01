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

#include "MathVariant.hh"
#include "MathVariantMap.hh"

#define MAP_NAME(n) map_variant_##n
#define DECLARE_MAP(n) Char32 MAP_NAME(n)(Char32);

typedef Char32 (*mapType)(Char32);

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

static Char32 map_variant_normal(Char32 ch)
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

void
mapMathVariant(MathVariant variant, UCS4String& str)
{
  assert(variant >= NORMAL_VARIANT && variant <= MONOSPACE_VARIANT);
  mapType m = map[variant - NORMAL_VARIANT];

  for (auto & elem : str)
    elem = m(elem);
}
