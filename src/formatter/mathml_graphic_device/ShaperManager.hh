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

#include "String.hh"
#include "GlyphSpec.hh"
#include "SparseMap.hh"

class ShaperManager : public Object
{
protected:
  ShaperManager(void);
  virtual ~ShaperManager();

public:
  static SmartPtr<ShaperManager> create(void)
  { return new ShaperManager(); }

  AreaRef shape(const class MathFormattingContext&, const UCS4String&) const;
  AreaRef shapeStretchy(const class MathFormattingContext&, const UCS4String&,
			const scaled& = 0, const scaled& = 0) const;
  
  unsigned registerShaper(const SmartPtr<class Shaper>&);
  void unregisterShapers(void);
  GlyphSpec registerChar(Char32 ch, const GlyphSpec& spec);
  GlyphSpec registerStretchyChar(Char32 ch, const GlyphSpec& spec);

private:
  GlyphSpec map(Char32 ch) const;
  GlyphSpec mapStretchy(Char32 ch) const;
  AreaRef shapeAux(const class MathFormattingContext&, class ShapingResult&) const;
  SmartPtr<class Shaper> getShaper(unsigned) const;

  static const unsigned MAX_SHAPERS = 16;
  static const unsigned HIGH_BITS = 16;
  static const unsigned LOW_BITS = 8;
  static const unsigned STRETCHY_BIT = LOW_BITS + HIGH_BITS;
  static const unsigned STRETCHY_FLAG = 1 << STRETCHY_BIT;
  static const Char32 BIGGEST_CHAR = 1 << (HIGH_BITS + 1 + LOW_BITS) - 1;

  SparseMap<GlyphSpec, HIGH_BITS + 1, LOW_BITS> glyphSpec;

  unsigned nextShaperId;
  SmartPtr<class Shaper> shaper[MAX_SHAPERS];
};

#endif // __ShaperManager_hh__
