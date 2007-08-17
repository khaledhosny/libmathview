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

#include <cassert>

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "FormattingContext.hh"
#include "MathMLElement.hh"
#include "SVG_TFMComputerModernMathGraphicDevice.hh"
#include "SVG_WrapperArea.hh"
#include "SVG_TFMGlyphArea.hh"
#include "TFMFont.hh"
#include "TFM.hh"


SVG_TFMComputerModernMathGraphicDevice::SVG_TFMComputerModernMathGraphicDevice(const SmartPtr<AbstractLogger>& l,
									       const SmartPtr<Configuration>&)
  : TFMComputerModernMathGraphicDevice(l)
{ }

SVG_TFMComputerModernMathGraphicDevice::~SVG_TFMComputerModernMathGraphicDevice()
{ }

SmartPtr<SVG_TFMComputerModernMathGraphicDevice>
SVG_TFMComputerModernMathGraphicDevice::create(const SmartPtr<AbstractLogger>& logger,
					       const SmartPtr<Configuration>& conf)
{ return new SVG_TFMComputerModernMathGraphicDevice(logger, conf); }

AreaRef
SVG_TFMComputerModernMathGraphicDevice::wrapper(const FormattingContext& ctxt, const AreaRef& area) const
{ return SVG_WrapperArea::create(area, area->box(), ctxt.getMathMLElement()); }

AreaRef
SVG_TFMComputerModernMathGraphicDevice::script(const class FormattingContext& context,
					       const AreaRef& base,
					       const AreaRef& subScript, const Length& subScriptShift,
					       const AreaRef& superScript, const Length& superScriptShift) const
{
  AreaRef nucleus = base;
  while (nucleus && is_a<const BinContainerArea>(nucleus))
    nucleus = smart_cast<const BinContainerArea>(nucleus)->getChild();

  AreaRef newSuperScript = superScript;
  if (superScript)
    if (SmartPtr<const SVG_TFMGlyphArea> glyph = smart_cast<const SVG_TFMGlyphArea>(nucleus))
      {
	const SmartPtr<TFMFont> font = glyph->getFont();
	const SmartPtr<TFM> tfm = font->getTFM();
	const Char8 index = glyph->getIndex();
	const scaled ic = tfm->getGlyphItalicCorrection(index) * tfm->getScale(font->getSize());
	if (ic != scaled::zero())
	  {
	    std::vector<AreaRef> c;
	    c.reserve(2);
	    c.push_back(getFactory()->horizontalSpace(ic));
	    c.push_back(superScript);
	    newSuperScript = getFactory()->horizontalArray(c);
	  }
      }

  return MathGraphicDevice::script(context, base,
				   subScript, subScriptShift,
				   newSuperScript, superScriptShift);
}

AreaRef
SVG_TFMComputerModernMathGraphicDevice::multiScripts(const class FormattingContext& context,
						     const AreaRef& base,
						     const std::vector<AreaRef>& subScripts,
						     const std::vector<AreaRef>& preSubScripts,
						     const Length& subScriptShift,
						     const std::vector<AreaRef>& superScripts,
						     const std::vector<AreaRef>& preSuperScripts,
						     const Length& superScriptShift) const
{
  return MathGraphicDevice::multiScripts(context, base,
					 subScripts, preSubScripts, subScriptShift,
					 superScripts, preSuperScripts, superScriptShift);
}
