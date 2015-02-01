// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#include <config.h>

#include <cmath>

#include "MathMLElement.hh"
#include "MathGraphicDevice.hh"
#include "FormattingContext.hh"

#include <iostream>

FormattingContext::FormattingContext(const SmartPtr<MathGraphicDevice>& md)
  : mathGraphicDevice(md)
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
  setStretchOperator(nullptr);
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

