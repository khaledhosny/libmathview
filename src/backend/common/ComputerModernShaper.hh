// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

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

  static UChar8 toTTFGlyphIndex(ComputerModernFamily::FontEncId, UChar8);

protected:
  virtual void postShape(class ShapingContext&) const;
  virtual AreaRef getGlyphArea(ComputerModernFamily::FontNameId,
			       ComputerModernFamily::FontSizeId, UChar8, int) const = 0;
  AreaRef getGlyphArea(MathVariant, ComputerModernFamily::FontEncId, UChar8, const scaled&) const;
  AreaRef getGlyphArea(MathVariant, const GlyphIndex&, const scaled&) const;

  AreaRef shapeChar(const class ShapingContext&, ComputerModernFamily::FontEncId) const;
  AreaRef shapeStretchyCharV(const class ShapingContext&) const;
  AreaRef shapeStretchyCharH(const class ShapingContext&) const;
  AreaRef shapeBigCharH(const class ShapingContext&, const ComputerModernShaper::HBigChar&) const;
  AreaRef shapeHorizontalBrace(const class ShapingContext&) const;
  AreaRef shapeHorizontalBar(const class ShapingContext&) const;
  AreaRef shapeSpecialChar(const class ShapingContext&) const;

private:
  PostShapingMode postShapingMode;
  SmartPtr<ComputerModernFamily> family;
};

#endif // __ComputerModernShaper_hh__
