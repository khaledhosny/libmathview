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

#ifndef __ShaperManager_hh__
#define __ShaperManager_hh__

#include "String.hh"
#include "scaled.hh"
#include "GlyphSpec.hh"
#include "SparseMap.hh"
#include "Area.hh"
#include "Object.hh"
#include "SmartPtr.hh"

class ShaperManager : public Object
{
protected:
  ShaperManager(void);
  virtual ~ShaperManager();

public:
  static SmartPtr<ShaperManager> create(void);

  SmartPtr<const class Area> shape(const class FormattingContext&,
				   const String&) const;
  SmartPtr<const class Area> shapeStretchy(const class FormattingContext&,
					   const String&,
					   const scaled& = 0, const scaled& = 0) const;
  
  unsigned registerShaper(const SmartPtr<class Shaper>&);
  void unregisterShapers(void);
  GlyphSpec registerChar(Char32 ch, const GlyphSpec& spec);
  GlyphSpec registerStretchyChar(Char32 ch, const GlyphSpec& spec);
  AreaRef compose(const FormattingContext& context,
	          const AreaRef base, const UCS4String baseSource,
	          const AreaRef script, const UCS4String scriptSource,
	          bool overScript);

private:
  const GlyphSpec& map(Char32 ch) const;
  const GlyphSpec& mapStretchy(Char32 ch) const;
  SmartPtr<const class Area> shapeAux(class ShapingContext&) const;
  SmartPtr<class Shaper> getShaper(unsigned) const;

  static const unsigned MAX_SHAPERS = 16;
  static const unsigned HIGH_BITS = 16;
  static const unsigned LOW_BITS = 8;
  static const unsigned STRETCHY_BIT = LOW_BITS + HIGH_BITS;
  static const unsigned STRETCHY_FLAG = 1 << STRETCHY_BIT;
  static const Char32 BIGGEST_CHAR = (1 << (HIGH_BITS + 1 + LOW_BITS)) - 1;

  SparseMap<GlyphSpec, HIGH_BITS + 1, LOW_BITS> glyphSpec;

  unsigned nextShaperId;
  SmartPtr<class Shaper> errorShaper;
  SmartPtr<class Shaper> shaper[MAX_SHAPERS];
};

#endif // __ShaperManager_hh__
