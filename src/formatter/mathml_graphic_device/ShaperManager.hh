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

#ifndef __ShaperManager_hh__
#define __ShaperManager_hh__

#include "DOM.hh"
#include "Shaper.hh"
#include "GlyphSpec.hh"
#include "SparseMap.hh"

class ShaperManager
{
public:
  ShaperManager(void);

  AreaRef shape(const SmartPtr<AreaFactory>&,
		const DOM::UCS4String&, const scaled&) const;
  AreaRef shapeStretchy(const SmartPtr<AreaFactory>&,
			const DOM::UCS4String&, const scaled&,
			const scaled& = 0, const scaled& = 0) const;
  
  unsigned registerShaper(const class Shaper&);
  GlyphSpec registerChar(DOM::Char32 ch, const GlyphSpec& spec);
  GlyphSpec registerStretchyChar(DOM::Char32 ch, const GlyphSpec& spec);

private:
  GlyphSpec map(DOM::Char32 ch) const;
  GlyphSpec mapStretchy(DOM::Char32 ch) const;
  AreaRef shapeAux(ShapingResult&) const;
  const class Shaper& getShaper(unsigned) const;

  static const unsigned MAX_SHAPERS = 16;
  static const unsigned HIGH_BITS = 12;
  static const unsigned LOW_BITS = 8;
  static const unsigned STRETCHY_BIT = LOW_BITS + HIGH_BITS;
  static const unsigned STRETCHY_FLAG = 1 << STRETCHY_BIT;
  static const DOM::Char32 BIGGEST_CHAR = 1 << (HIGH_BITS + LOW_BITS) - 1;

  SparseMap<GlyphSpec, HIGH_BITS, LOW_BITS> glyphSpec;

  unsigned nextShaperId;
  const class Shaper* shaper[MAX_SHAPERS];
};

#endif // __ShaperManager_hh__
