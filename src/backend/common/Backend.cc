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

#include "AbstractLogger.hh"
#include "Backend.hh"
#include "ShaperManager.hh"
#include "MathGraphicDevice.hh"
#if GMV_ENABLE_BOXML
#include "BoxGraphicDevice.hh"
#endif // GMV_ENABLE_BOXML

Backend::Backend(const SmartPtr<AbstractLogger>& logger,
		 const SmartPtr<Configuration>&)
  : shaperManager(ShaperManager::create(logger))
{ }

Backend::~Backend()
{
  shaperManager->unregisterShapers();
}

SmartPtr<ShaperManager>
Backend::getShaperManager() const
{ return shaperManager; }

void
Backend::setMathGraphicDevice(const SmartPtr<MathGraphicDevice>& mgd)
{
  mathGraphicDevice = mgd;
  if (mathGraphicDevice)
    mathGraphicDevice->setShaperManager(shaperManager);
}

SmartPtr<MathGraphicDevice>
Backend::getMathGraphicDevice() const
{ return mathGraphicDevice; }

#if GMV_ENABLE_BOXML
void
Backend::setBoxGraphicDevice(const SmartPtr<BoxGraphicDevice>& bgd)
{
  boxGraphicDevice = bgd;
  if (boxGraphicDevice)
    boxGraphicDevice->setShaperManager(shaperManager);
}

SmartPtr<BoxGraphicDevice>
Backend::getBoxGraphicDevice() const
{ return boxGraphicDevice; }
#endif // GMV_ENABLE_BOXML

