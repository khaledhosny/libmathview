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

#ifndef __ShapingContext_hh__
#define __ShapingContext_hh__

#include <vector>

#include "String.hh"
#include "GlyphSpec.hh"
#include "Area.hh"
#include "MathVariant.hh"

class GMV_MathView_EXPORT ShapingContext
{
public:
  ShapingContext(const SmartPtr<class Element>&,
		 const SmartPtr<class AreaFactory>&,
		 const UCS4String&,
		 const std::vector<GlyphSpec>&,
		 const scaled&,
		 MathVariant,
		 bool,
		 const scaled& = 0, const scaled& = 0);

  MathVariant getMathVariant(void) const { return mathVariant; }
  bool inMathMode(void) const { return mathMode; }
  SmartPtr<class Element> getElement(void) const;
  SmartPtr<class AreaFactory> getFactory(void) const;
  UCS4String getSource(void) const { return source; }
  bool done(void) const { return index == source.length(); }
  bool empty(void) const { return res.empty(); }
  scaled getSize(void) const { return size; }
  scaled getVSpan(void) const { return vSpan; }
  scaled getHSpan(void) const { return hSpan; }
  unsigned chunkSize(void) const;
  unsigned getShaperId(void) const;

  unsigned nAreas(void) const { return res.size(); }
  AreaRef popArea(CharIndex&);
  void pushArea(CharIndex, const AreaRef&);
  AreaRef getArea(int = -1) const;
  AreaRef area(void) const;

  unsigned getIndex(void) const { return index; }

  const Char32* data(void) const;
  const GlyphSpec& getSpec(int = 0) const;
  Char32 prevChar(void) const;
  Char32 thisChar(void) const;
  Char32 nextChar(void) const;
  UCS4String prevString(void) const;
  UCS4String prevString(UCS4String::size_type) const;
  UCS4String nextString(void) const;
  UCS4String nextString(UCS4String::size_type) const;

private:
  SmartPtr<class Element> element;
  SmartPtr<class AreaFactory> factory;
  UCS4String source;
  std::vector<GlyphSpec> spec;
  scaled size;
  MathVariant mathVariant;
  bool mathMode;
  scaled vSpan;
  scaled hSpan;
  UCS4String::size_type index;
  std::vector<CharIndex> res_n;
  std::vector<AreaRef> res;
};

#endif // __ShapingContext_hh__
