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

#include <config.h>

#include <cmath>

#include "MathMLElement.hh"
#include "BoxMLElement.hh"
#include "MathGraphicDevice.hh"
#include "BoxGraphicDevice.hh"
#include "FormattingContext.hh"

#include <iostream>

#if GMV_ENABLE_BOXML
FormattingContext::FormattingContext(const SmartPtr<MathGraphicDevice>& md,
				     const SmartPtr<BoxGraphicDevice>& bd)
  : mathGraphicDevice(md), boxGraphicDevice(bd)
#else
FormattingContext::FormattingContext(const SmartPtr<MathGraphicDevice>& md)
  : mathGraphicDevice(md)
#endif
{
  setMathMode(true);
  setSize(mathGraphicDevice->evaluate(*this, Length(10.0, Length::PT_UNIT), scaled::zero()));
  setActualSize(getSize());
  setVariant(NORMAL_VARIANT);
  setColor(RGBColor::BLACK());
  setBackground(RGBColor::WHITE());
  set<int>(SCRIPT_LEVEL, 0);
  setMinSize(mathGraphicDevice->evaluate(*this, Length(6.0, Length::PT_UNIT), scaled::zero()));
  setDisplayStyle(false);
  setSizeMultiplier(0.71);

  for (int i = -7; i <= 7; i++)
    setMathSpace(ZERO_SPACE + i, Length(i / 18.0, Length::EM_UNIT));

  setAvailableWidth(scaled::zero());
  setStretchOperator(0);
  setStretchToWidth(scaled::zero());
  setStretchToHeight(scaled::zero());
  setStretchToDepth(scaled::zero());
  setStretchH(scaled::zero());
  setStretchV(scaled::zero());
}

FormattingContext::~FormattingContext()
{ }

void
FormattingContext::addScriptLevel(int dl)
{
  scaled aSize = getActualSize() * pow(getSizeMultiplier(), dl);
  setActualSize(aSize);
  setSize(std::max(getMinSize(), aSize));
  set<int>(SCRIPT_LEVEL, getScriptLevel() + dl);
}

SmartPtr<class MathMLElement>
FormattingContext::getStretchOperator() const
{ return get< SmartPtr<MathMLElement> >(STRETCH_OP); }

void
FormattingContext::setStretchOperator(const SmartPtr<MathMLElement>& op)
{ set< SmartPtr<MathMLElement> >(STRETCH_OP, op); }

void
FormattingContext::push(const SmartPtr<MathMLElement>& el)
{
  push();
  set(MATHML_ELEMENT, el);
}

SmartPtr<MathMLElement> 
FormattingContext::getMathMLElement() const
{ return get<SmartPtr<MathMLElement> >(MATHML_ELEMENT); }

SmartPtr<MathGraphicDevice>
FormattingContext::MGD() const
{ return mathGraphicDevice; }

#if GMV_ENABLE_BOXML
void
FormattingContext::push(const SmartPtr<BoxMLElement>& el)
{
  push();
  set(BOXML_ELEMENT, el);
}

SmartPtr<BoxMLElement> 
FormattingContext::getBoxMLElement() const
{ return get< SmartPtr<BoxMLElement> >(BOXML_ELEMENT); }

SmartPtr<BoxGraphicDevice>
FormattingContext::BGD() const
{ return boxGraphicDevice; }
#endif // GMV_ENABLE_BOXML
