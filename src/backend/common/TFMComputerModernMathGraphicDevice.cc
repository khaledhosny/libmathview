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

