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

#include <config.h>

#include <cmath>

#include "MathMLElement.hh"
#include "MathGraphicDevice.hh"
#include "MathFormattingContext.hh"

MathFormattingContext::MathFormattingContext(const SmartPtr<MathGraphicDevice>& d)
  : device(d)
{
  setZoom(100);
  setSize(device->evaluate(*this, Length(10.0, Length::PT_UNIT), scaled::zero()));
  setActualSize(getSize());
  setVariant(NORMAL_VARIANT);
  setColor(RGBColor::BLACK());
  setBackground(RGBColor::WHITE());
  set<int>(SCRIPT_LEVEL, 0);
  setMinSize(device->evaluate(*this, Length(6.0, Length::PT_UNIT), scaled::zero()));
  setDisplayStyle(false);
  setSizeMultiplier(0.71);

  for (int i = -7; i <= 7; i++)
    setMathSpace(ZERO_SPACE + i, Length(i / 18.0, Length::EM_UNIT));

  setStretchOperator(0);
  setStretchToWidth(scaled::zero());
  setStretchToHeight(scaled::zero());
  setStretchToDepth(scaled::zero());
  setStretchH(scaled::zero());
  setStretchV(scaled::zero());
}

SmartPtr<MathGraphicDevice>
MathFormattingContext::getDevice() const
{
  return device;
}

void
MathFormattingContext::addScriptLevel(int dl)
{
  scaled aSize = getActualSize() * pow(getSizeMultiplier(), dl);
  setActualSize(aSize);
  setSize(std::max(getMinSize(), aSize));
  set<int>(SCRIPT_LEVEL, getScriptLevel() + dl);
}
