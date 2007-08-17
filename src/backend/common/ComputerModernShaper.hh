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

#ifndef __ComputerModernShaper_hh__
#define __ComputerModernShaper_hh__

#include "Char.hh"
#include "Shaper.hh"
#include "ComputerModernFamily.hh"

class GMV_MathView_EXPORT ComputerModernShaper : public Shaper
{
protected:
  ComputerModernShaper(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&);
  virtual ~ComputerModernShaper();

public:
  SmartPtr<ComputerModernFamily> getFamily(void) const { return family; }

  virtual void registerShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual void unregisterShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual void shape(class ShapingContext&) const;

  enum PostShapingMode {
    POST_SHAPING_NEVER,
    POST_SHAPING_MATH,
    POST_SHAPING_TEXT,
    POST_SHAPING_ALWAYS
  };
  
  PostShapingMode getPostShapingMode(void) const { return postShapingMode; }
  void setPostShapingMode(PostShapingMode m) { postShapingMode = m; }
  bool setPostShapingMode(const String&);

  struct GlyphIndex
  {
    signed char fontEnc;
    UChar8 index;
    bool valid(void) const
    { return ComputerModernFamily::validFontEncId(ComputerModernFamily::FontEncId(fontEnc)); }
  };
  
  struct PlainChar
  {
    Char16 ch;
    UChar8 index;
  };

  struct PlainChar32
  {
    Char32 ch;
    UChar8 index;
  };

  struct HStretchyChar
  {
    Char16 ch;
    GlyphIndex normal;
    GlyphIndex left;
    GlyphIndex glue;
    GlyphIndex right;
  };
  
  struct VStretchyChar
  {
    Char16 ch;
    GlyphIndex normal[5];
    GlyphIndex top;
    GlyphIndex glue;
    GlyphIndex middle;
    GlyphIndex bottom;
  };

  struct HBigChar
  {
    Char16 ch;
    GlyphIndex big[5];
  };

  struct CombiningChar
  {
    Char16 ch;
    GlyphIndex spec;
  };

  static UChar8 toTTFGlyphIndex(ComputerModernFamily::FontEncId, UChar8);

protected:
  virtual void postShape(class ShapingContext&) const;
  virtual AreaRef getGlyphArea(ComputerModernFamily::FontNameId,
			       ComputerModernFamily::FontSizeId, UChar8, int) const = 0;
  AreaRef getGlyphArea(MathVariant, ComputerModernFamily::FontEncId, UChar8, const scaled&) const;
  AreaRef getGlyphArea(MathVariant, const GlyphIndex&, const scaled&) const;

  bool shapeChar(class ShapingContext&, ComputerModernFamily::FontEncId) const;
  bool shapeStretchyCharV(class ShapingContext&) const;
  bool shapeStretchyCharH(class ShapingContext&) const;
  bool shapeBigCharH(class ShapingContext&, const ComputerModernShaper::HBigChar&) const;
  bool shapeHorizontalBrace(class ShapingContext&) const;
  bool shapeHorizontalBar(class ShapingContext&) const;
  bool shapeSpecialStretchyChar(class ShapingContext&) const;
  virtual bool shapeCombiningChar(class ShapingContext&) const;

protected:
  PostShapingMode postShapingMode;
  SmartPtr<ComputerModernFamily> family;
};

#endif // __ComputerModernShaper_hh__
