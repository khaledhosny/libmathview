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
#include "FormattingContext.hh"
#include "SVG_BoxGraphicDevice.hh"
#include "BoxMLElement.hh"
#include "SVG_WrapperArea.hh"

SVG_BoxGraphicDevice::SVG_BoxGraphicDevice(const SmartPtr<AbstractLogger>& l,
					   const SmartPtr<Configuration>& conf)
  : BoxGraphicDevice(l)
{ }

SVG_BoxGraphicDevice::~SVG_BoxGraphicDevice()
{ }

SmartPtr<SVG_BoxGraphicDevice>
SVG_BoxGraphicDevice::create(const SmartPtr<AbstractLogger>& logger,
			     const SmartPtr<Configuration>& conf)
{ return new SVG_BoxGraphicDevice(logger, conf); }

AreaRef
SVG_BoxGraphicDevice::wrapper(const FormattingContext& ctxt, const AreaRef& area) const
{ return SVG_WrapperArea::create(area, area->box(), ctxt.getBoxMLElement()); }
