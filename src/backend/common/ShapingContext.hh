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

class ShapingContext
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

  MathVariant getMathVariant(void) const { return m_mathVariant; }
  bool inMathMode(void) const { return m_mathMode; }
  SmartPtr<class Element> getElement(void) const;
  SmartPtr<class AreaFactory> getFactory(void) const;
  UCS4String getSource(void) const { return m_source; }
  bool done(void) const { return m_index == m_source.length(); }
  bool empty(void) const { return m_res.empty(); }
  scaled getSize(void) const { return m_size; }
  scaled getVSpan(void) const { return m_vSpan; }
  scaled getHSpan(void) const { return m_hSpan; }
  unsigned chunkSize(void) const;
  unsigned getShaperId(void) const;

  unsigned nAreas(void) const { return m_res.size(); }
  AreaRef popArea(CharIndex&);
  void pushArea(CharIndex, const AreaRef&);
  AreaRef getArea(int = -1) const;
  AreaRef area(void) const;

  unsigned getIndex(void) const { return m_index; }

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
  SmartPtr<class Element> m_element;
  SmartPtr<class AreaFactory> m_factory;
  UCS4String m_source;
  std::vector<GlyphSpec> m_spec;
  scaled m_size;
  MathVariant m_mathVariant;
  bool m_mathMode;
  scaled m_vSpan;
  scaled m_hSpan;
  UCS4String::size_type m_index;
  std::vector<CharIndex> m_res_n;
  std::vector<AreaRef> m_res;
};

#endif // __ShapingContext_hh__
