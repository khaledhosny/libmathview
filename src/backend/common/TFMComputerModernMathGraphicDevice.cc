// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include <config.h>

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "ComputerModernFamily.hh"
#include "TFMManager.hh"
#include "TFMComputerModernMathGraphicDevice.hh"
#include "FormattingContext.hh"

TFMComputerModernMathGraphicDevice::TFMComputerModernMathGraphicDevice(const SmartPtr<AbstractLogger>& l)
  : MathGraphicDevice(l)
{ }

TFMComputerModernMathGraphicDevice::~TFMComputerModernMathGraphicDevice()
{ }

void
TFMComputerModernMathGraphicDevice::setFamily(const SmartPtr<ComputerModernFamily>& f)
{ family = f; }

void
TFMComputerModernMathGraphicDevice::setTFMManager(const SmartPtr<TFMManager>& m)
{ tfmManager = m; }

SmartPtr<TFMComputerModernMathGraphicDevice>
TFMComputerModernMathGraphicDevice::create(const SmartPtr<AbstractLogger>& l)
{ return new TFMComputerModernMathGraphicDevice(l); }

SmartPtr<TFM>
TFMComputerModernMathGraphicDevice::getTFM(const FormattingContext& context,
					   ComputerModernFamily::FontEncId encId) const
{
  const String fontName = family->nameOfFont(context.getVariant(), encId, context.getSize());
  return tfmManager->getTFM(fontName);
}

scaled
TFMComputerModernMathGraphicDevice::em(const FormattingContext& context) const
{
  if (SmartPtr<TFM> tfm = getTFM(context, ComputerModernFamily::FE_CMSY))
    return tfm->getDimension(6) * tfm->getScale(context.getSize());
  else
    return MathGraphicDevice::em(context);
}

scaled
TFMComputerModernMathGraphicDevice::ex(const FormattingContext& context) const
{
  if (SmartPtr<TFM> tfm = getTFM(context, ComputerModernFamily::FE_CMSY))
    return tfm->getDimension(5) * tfm->getScale(context.getSize());
  else
    return MathGraphicDevice::ex(context);
}

scaled
TFMComputerModernMathGraphicDevice::defaultLineThickness(const FormattingContext& context) const
{
  if (SmartPtr<TFM> tfm = getTFM(context, ComputerModernFamily::FE_CMEX))
    return tfm->getDimension(8) * tfm->getScale(context.getSize());
  else
    return MathGraphicDevice::defaultLineThickness(context);
}

scaled
TFMComputerModernMathGraphicDevice::axis(const FormattingContext& context) const
{
  if (SmartPtr<TFM> tfm = getTFM(context, ComputerModernFamily::FE_CMSY))
    return tfm->getDimension(22) * tfm->getScale(context.getSize());
  else
    return MathGraphicDevice::axis(context);
}

AreaRef
TFMComputerModernMathGraphicDevice::glyph(const class FormattingContext& context,
					  const String& alt, const String& family,
					  unsigned long index) const
{
  return MathGraphicDevice::glyph(context, alt, family, index);
}

