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
#include "MathVariant.hh"

class ComputerModernShaper : public Shaper
{
protected:
  ComputerModernShaper(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&);
  virtual ~ComputerModernShaper();

public:
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

  enum FontSizeId {
    FS_NIL = -1,
    FS_5,
    FS_6,
    FS_7,
    FS_8,
    FS_9,
    FS_10,
    FS_12,
    FS_17,
    FS_NOT_VALID
  };

  enum FontEncId {
    FE_NIL = -1,
    FE_CMR,
    FE_CMMI,
    FE_CMSY,
    FE_CMEX,
    FE_H_STRETCHY,
    FE_V_STRETCHY,
    FE_H_BIG,
    FE_NOT_VALID
  };

  enum FontNameId {
    FN_NIL = -1,
    FN_CMR,
    FN_CMB,
    FN_CMBX,
    FN_CMBXTI,
    FN_CMTI,
    FN_CMSS,
    FN_CMSSI,
    FN_CMSSBX,
    FN_CMTT,
    FN_CMSY,
    FN_CMBSY,
    FN_CMMI,
    FN_CMMIB, 
    FN_CMEX,
    FN_NOT_VALID
  };

  struct GlyphIndex
  {
    signed char fontEnc;
    Char8 index;
    bool valid(void) const { return fontEnc > FE_NIL && fontEnc < FE_NOT_VALID; }
  };
  
  struct PlainChar
  {
    Char16 ch;
    Char8  index;
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

  static const char* nameOfFontNameId(FontNameId);
  static String nameOfFont(FontNameId, FontSizeId);
  static Char8 toTTFGlyphIndex(FontEncId, Char8);

protected:
  static FontEncId encIdOfFontNameId(FontNameId);
  static FontSizeId fontSizeIdOfSize(int);
  static int sizeOfFontSizeId(FontSizeId);

  bool fontEnabled(FontNameId, FontSizeId = FS_10) const;

  FontNameId findBestFont(MathVariant, FontEncId, int, FontSizeId&) const;
  FontNameId findFont(MathVariant, FontEncId, scaled&, FontSizeId&) const;

  virtual void postShape(class ShapingContext&) const;
  virtual AreaRef getGlyphArea(FontNameId, FontSizeId, Char8, int) const = 0;
  AreaRef getGlyphArea(MathVariant, FontEncId, Char8, const scaled&) const;
  AreaRef getGlyphArea(MathVariant, const GlyphIndex&, const scaled&) const;

  AreaRef shapeChar(const class ShapingContext&, FontEncId) const;
  AreaRef shapeStretchyCharV(const class ShapingContext&) const;
  AreaRef shapeStretchyCharH(const class ShapingContext&) const;

private:
  PostShapingMode postShapingMode;
  bool configuredFont[FN_NOT_VALID][FS_NOT_VALID];
};

#endif // __ComputerModernShaper_hh__
