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

#ifndef __CachedShapedString_hh__
#define __CachedShapedString_hh__

#include "String.hh"
#include "StringHash.hh"

struct CachedShapedStringKey
{
  CachedShapedStringKey(const String& s, MathVariant v, const scaled& sz)
    : source(s), variant(v), size(sz) { }

  bool operator==(const CachedShapedStringKey& key) const
  { return source == key.source && variant == key.variant && size == key.size; }

  String source;
  MathVariant variant;
  scaled size;
};

struct CachedShapedStringKeyHash
{
  size_t operator()(const CachedShapedStringKey& key) const
  { return StringHash()(key.source) ^ key.variant ^ key.size.getValue(); }
};

struct CachedShapedStretchyStringKey : public CachedShapedStringKey
{
  CachedShapedStretchyStringKey(const String& s,
				MathVariant v,
				const scaled& sz,
				const scaled& sh,
				const scaled& sv)
    : CachedShapedStringKey(s, v, sz), spanH(sh), spanV(sv) { }

  bool operator==(const CachedShapedStretchyStringKey& key) const
  { return source == key.source && variant == key.variant && size == key.size
      && spanH == key.spanH && spanV == key.spanV; }

  scaled spanH;
  scaled spanV;
};

struct CachedShapedStretchyStringKeyHash
{
  size_t operator()(const CachedShapedStretchyStringKey& key) const
  { return StringHash()(key.source) ^ key.variant ^ key.size.getValue()
      ^ key.spanH.getValue() ^ key.spanV.getValue(); }
};

#endif // __CachedShapedString_hh__
