// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef RenderingEnvironment_hh
#define RenderingEnvironment_hh

#include <list>

#include "Length.hh"
#include "MathMLAttribute.hh"
#include "MathMLAttributeList.hh"
#include "RGBColor.hh"
#include "String.hh"
#include "scaled.hh"
#include "token.hh"

#define MAX_FONT_FAMILY 64

class RenderingEnvironment {
private:
  struct AttributeLevel;

public:
  RenderingEnvironment(const SmartPtr<class AreaFactory>&,
		       const SmartPtr<class ShaperManager>&);
  virtual ~RenderingEnvironment();

  // levels
  void     Push(void);
  void     Drop(void);

  // attributes facilities
  void     SetDisplayStyle(bool);
  bool     GetDisplayStyle(void) const;
  void     SetScriptSizeMultiplier(float);
  void     SetScriptMinSize(const Length&);
  void     AddScriptLevel(int);
  void     SetScriptLevel(int);
  int      GetScriptLevel(void) const;

  // font specific attributes
  void     SetFontSize(const Length&);
  const Length& GetFontSize(void) const;

  // math spaces
  enum MathSpaceId {
    MATH_SPACE_NEGATIVEVERYVERYTHICK,
    MATH_SPACE_NEGATIVEVERYTHICK,
    MATH_SPACE_NEGATIVETHICK,
    MATH_SPACE_NEGATIVEMEDIUM,
    MATH_SPACE_NEGATIVETHIN,
    MATH_SPACE_NEGATIVEVERYTHIN,
    MATH_SPACE_NEGATIVEVERYVERYTHIN,
    MATH_SPACE_VERYVERYTHIN,
    MATH_SPACE_VERYTHIN,
    MATH_SPACE_THIN,
    MATH_SPACE_MEDIUM,
    MATH_SPACE_THICK,
    MATH_SPACE_VERYTHICK,
    MATH_SPACE_VERYVERYTHICK,

    MATH_SPACE_LAST
  };

  static MathSpaceId mathSpaceIdOfTokenId(TokenId);
  const Length& GetMathSpace(MathSpaceId) const;
  void SetMathSpace(MathSpaceId, const Length&);

  // colors
  void     SetColor(const RGBColor&);
  void     SetBackgroundColor(const RGBColor&);
  RGBColor GetColor(void) const;
  RGBColor GetBackgroundColor(void) const;

  // conversions
  scaled   GetScaledPointsPerEm(void) const;
  scaled   GetScaledPointsPerEx(void) const;
  scaled   ToScaledPoints(const Length&) const;
  scaled   GetAxis(void) const;
  scaled   GetRuleThickness(void) const;

  SmartPtr<class AreaFactory> areaFactory;
  SmartPtr<class ShaperManager> shaperManager;

private:
  struct AttributeLevel {
    bool      displayStyle;
    
    int       scriptLevel;      // Indeed scriptLevel can be negative!
    Length scriptMinSize;
    float     scriptSizeMultiplier;

    Length size;
    
    Length mathSpace[14];
    
    RGBColor  color;
    RGBColor  background;
    bool      transparentBackground;
  };

  std::list<AttributeLevel*> level;
};

#endif // RenderingEnvironment_hh
