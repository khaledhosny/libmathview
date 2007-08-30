// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __ShaperManager_hh__
#define __ShaperManager_hh__

#include "String.hh"
#include "scaled.hh"
#include "GlyphSpec.hh"
#include "SparseMap.hh"
#include "Area.hh"
#include "Object.hh"
#include "SmartPtr.hh"

class GMV_MathView_EXPORT ShaperManager : public Object
{
protected:
  ShaperManager(const SmartPtr<class AbstractLogger>&);
  virtual ~ShaperManager();

public:
  static SmartPtr<ShaperManager> create(const SmartPtr<class AbstractLogger>&);

  SmartPtr<const class Area> shape(const class FormattingContext&,
				   const SmartPtr<class Element>&,
				   const SmartPtr<class AreaFactory>&,
				   const UCS4String&) const;
  SmartPtr<const class Area> shapeStretchy(const class FormattingContext&,
					   const SmartPtr<class Element>&,
					   const SmartPtr<class AreaFactory>&,
					   const UCS4String&,
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
  SmartPtr<class AbstractLogger> logger;
  SmartPtr<class Shaper> errorShaper;
  SmartPtr<class Shaper> shaper[MAX_SHAPERS];
};

#endif // __ShaperManager_hh__
