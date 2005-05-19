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

class ComputerModernShaper : public Shaper
{
protected:
  ComputerModernShaper(void);
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

  enum FontNameId { FN_NIL, FN_CMR, FN_CMB, FN_CMBXTI, FN_CMTI, FN_CMSS, FN_CMSSI,
		    FN_CMSSBX, FN_CMTT, FN_CMSY, FN_CMBSY, FN_CMMI, FN_CMMIB, 
		    FN_CMEX, FN_NOT_VALID };

  struct GlyphIndex
  {
    Char8 fontName;
    Char8 index;
    bool valid(void) const { return fontName > FN_NIL && fontName < FN_NOT_VALID; }
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

  static const char* nameOfFont(FontNameId);
  static Char8 toTTFGlyphIndex(FontNameId, Char8);

protected:
  virtual void postShape(class ShapingContext&) const;
  virtual AreaRef getGlyphArea(const SmartPtr<class AreaFactory>&, FontNameId, Char8, const scaled&) const = 0;
  AreaRef getGlyphArea(const SmartPtr<class AreaFactory>&, const GlyphIndex&, const scaled&) const;

  AreaRef shapeChar(const class ShapingContext&, FontNameId) const;
  AreaRef shapeStretchyCharV(const class ShapingContext&) const;
  AreaRef shapeStretchyCharH(const class ShapingContext&) const;

private:
  PostShapingMode postShapingMode;
};

#endif // __ComputerModernShaper_hh__
