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

#include <stddef.h>

#include <list>

#include "AFont.hh"
#include "String.hh"
#include "RGBValue.hh"
#include "UnitValue.hh"
#include "FontAttributes.hh"
#include "MathMLDocument.hh"
#include "MathMLAttribute.hh"
#include "MathMLAttributeList.hh"

#define MAX_FONT_FAMILY 64

class RenderingEnvironment {
private:
  struct AttributeLevel;

public:
  RenderingEnvironment(class CharMapper&);
  virtual ~RenderingEnvironment();

  // levels
  void     Push(const MathMLAttributeList* = NULL);
  void     Drop(void);

  // document
  void     SetDocument(const Ptr<class MathMLDocument>&);
  Ptr<class MathMLDocument> GetDocument(void) const;

  // attributes retrieving
  const MathMLAttribute* GetAttribute(AttributeId) const;

  // attributes facilities
  void     SetDisplayStyle(bool);
  bool     GetDisplayStyle(void) const;
  void     SetScriptSizeMultiplier(float);
  void     SetScriptMinSize(const UnitValue&);
  void     AddScriptLevel(int);
  void     SetScriptLevel(int);
  int      GetScriptLevel(void) const;

  // font specific attributes
  void     SetFontFamily(const char*);
  void     SetFontFamily(const String*);
  void     SetFontSize(const UnitValue&);
  void     SetFontWeight(FontWeightId);
  void     SetFontStyle(FontStyleId);
  const FontAttributes& GetFontAttributes(void) const;
  void     SetFontMode(FontModeId);
  FontModeId GetFontMode(void) const;

  // math spaces
  const UnitValue& GetMathSpace(MathSpaceId) const;
  void     SetMathSpace(MathSpaceId, const UnitValue&);

  // colors
  void     SetColor(RGBValue);
  void     SetBackgroundColor(RGBValue);
  RGBValue GetColor(void) const;
  RGBValue GetBackgroundColor(void) const;

  // conversions
  scaled   GetScaledPointsPerEm(void) const;
  scaled   GetScaledPointsPerEx(void) const;
  scaled   ToScaledPoints(const UnitValue&) const;
  scaled   GetAxis(void) const;
  scaled   GetRuleThickness(void) const;

  class CharMapper& charMapper;

private:
  struct AttributeLevel {
    bool      displayStyle;
    
    int       scriptLevel;      // Indeed scriptLevel can be negative!
    UnitValue scriptMinSize;
    float     scriptSizeMultiplier;

    FontAttributes fontAttributes;
    
    UnitValue mathSpace[MATH_SPACE_LAST];
    
    RGBValue  color;
    RGBValue  background;
    bool      transparentBackground;
    Ptr<class MathMLDocument> doc;
    const MathMLAttributeList* defaults;
  };

  std::list<AttributeLevel*> level;
};

#endif // RenderingEnvironment_hh
