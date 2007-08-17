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

#ifndef __FormattingContext_hh__
#define __FormattingContext_hh__

#include "scaled.hh"
#include "Variant.hh"
#include "RGBColor.hh"
#include "Length.hh"
// full path needed for Win32
#include "../../common/mathvariants/MathVariant.hh"
#include "FastScopedHashMap.hh"

class GMV_MathView_EXPORT FormattingContext
{
public:
#if GMV_ENABLE_BOXML
  FormattingContext(const SmartPtr<class MathGraphicDevice>&,
		    const SmartPtr<class BoxGraphicDevice>&);
#else
  FormattingContext(const SmartPtr<class MathGraphicDevice>&);
#endif
  ~FormattingContext();

  enum PropertyId {
    MATH_MODE,
    SIZE,
    ACTUAL_SIZE,
    VARIANT,
    COLOR,
    BACKGROUND_COLOR,
    SCRIPT_LEVEL,
    MIN_SIZE,
    DISPLAY_STYLE,
    SIZE_MULT,
    MATHML_ELEMENT,
#if GMV_ENABLE_BOXML
    BOXML_ELEMENT,
#endif
    AVAILABLE_WIDTH,
    STRETCH_OP,
    STRETCH_TO_WIDTH,
    STRETCH_TO_HEIGHT,
    STRETCH_TO_DEPTH,
    STRETCH_HORIZ,
    STRETCH_VERT,
    NEGATIVE_VERYVERYTHICK_SPACE,
    NEGATIVE_VERYTHICK_SPACE,
    NEGATIVE_THICK_SPACE,
    NEGATIVE_MEDIUM_SPACE,
    NEGATIVE_THIN_SPACE,
    NEGATIVE_VERYTHIN_SPACE,
    NEGATIVE_VERYVERYTHIN_SPACE,
    ZERO_SPACE,
    VERYVERYTHIN_SPACE,
    VERYTHIN_SPACE,
    THIN_SPACE,
    MEDIUM_SPACE,
    THICK_SPACE,
    VERYTHICK_SPACE,
    VERYVERYTHICK_SPACE,
    
    LAST_NAMED_PROPERTY_ENTRY
  };

  bool getMathMode(void) const { return get<bool>(MATH_MODE); }
  void setMathMode(bool m) { set<bool>(MATH_MODE, m); }
  scaled getSize(void) const { return get<scaled>(SIZE); }
  void setSize(const scaled& s) { set<scaled>(SIZE, s); }
  scaled getActualSize(void) const { return get<scaled>(ACTUAL_SIZE); }
  void setActualSize(const scaled& s) { set<scaled>(ACTUAL_SIZE, s); }
  MathVariant getVariant(void) const { return get<MathVariant>(VARIANT); }
  void setVariant(MathVariant v) { set<MathVariant>(VARIANT, v); }
  RGBColor getColor(void) const { return get<RGBColor>(COLOR); }
  void setColor(const RGBColor& c) { set<RGBColor>(COLOR, c); }
  RGBColor getBackground(void) const { return get<RGBColor>(BACKGROUND_COLOR); }
  void setBackground(const RGBColor& c) { set<RGBColor>(BACKGROUND_COLOR, c); }
  int getScriptLevel(void) const { return get<int>(SCRIPT_LEVEL); }
  void setScriptLevel(int l) { addScriptLevel(l - getScriptLevel()); }
  void addScriptLevel(int);
  scaled getMinSize(void) const { return get<scaled>(MIN_SIZE); }
  void setMinSize(scaled s) { set<scaled>(MIN_SIZE, s); }
  bool getDisplayStyle(void) const { return get<bool>(DISPLAY_STYLE); }
  void setDisplayStyle(bool b) { set<bool>(DISPLAY_STYLE, b); }
  double getSizeMultiplier(void) const { return get<double>(SIZE_MULT); }
  void setSizeMultiplier(double f) { set<double>(SIZE_MULT, f); }
  Length getMathSpace(int i) const { return get<Length>(i); }
  void setMathSpace(int i, const Length& l) { set<Length>(i, l); }
  scaled getAvailableWidth(void) const { return get<scaled>(AVAILABLE_WIDTH); }
  void setAvailableWidth(const scaled& w) { set<scaled>(AVAILABLE_WIDTH, w); }
  SmartPtr<class MathMLElement> getStretchOperator(void) const;
  void setStretchOperator(const SmartPtr<class MathMLElement>&);
  scaled getStretchToWidth(void) const { return get<scaled>(STRETCH_TO_WIDTH); }
  void setStretchToWidth(const scaled& w) { set<scaled>(STRETCH_TO_WIDTH, w); }
  scaled getStretchToHeight(void) const { return get<scaled>(STRETCH_TO_HEIGHT); }
  void setStretchToHeight(const scaled& h) { set<scaled>(STRETCH_TO_HEIGHT, h); }
  scaled getStretchToDepth(void) const { return get<scaled>(STRETCH_TO_DEPTH); }
  void setStretchToDepth(const scaled& d) { set<scaled>(STRETCH_TO_DEPTH, d); }
  scaled getStretchH(void) const { return get<scaled>(STRETCH_HORIZ); }
  void setStretchH(const scaled& h) { set<scaled>(STRETCH_HORIZ, h); }
  scaled getStretchV(void) const { return get<scaled>(STRETCH_VERT); }
  void setStretchV(const scaled& v) { set<scaled>(STRETCH_VERT, v); }

  void push(const SmartPtr<class MathMLElement>&);
  SmartPtr<class MathMLElement> getMathMLElement(void) const;
  SmartPtr<class MathGraphicDevice> MGD(void) const;

#if GMV_ENABLE_BOXML  
  void push(const SmartPtr<class BoxMLElement>&);
  SmartPtr<class BoxMLElement> getBoxMLElement(void) const;
  SmartPtr<class BoxGraphicDevice> BGD(void) const;
#endif // GMV_ENABLE_BOXML

protected:
  template <typename T>
  void set(int id, const T& v)
  { map.set(id, Variant<T>::create(v)); }

  template <typename T>
  T get(int id) const
  { return as<T>(map.get(id)); }

public:
  void push(void)
  { map.push(); }

  void pop()
  { map.pop(); }

private:
  SmartPtr<class MathGraphicDevice> mathGraphicDevice;
#if GMV_ENABLE_BOXML
  SmartPtr<class BoxGraphicDevice> boxGraphicDevice;
#endif // GMV_ENABLE_BOXML
  FastScopedHashMap<LAST_NAMED_PROPERTY_ENTRY, SmartPtr<Value> > map;
};

#endif // __FormattingContext_hh__
