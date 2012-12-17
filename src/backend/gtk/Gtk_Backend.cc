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

#include <gtk/gtk.h>

#include <map>

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "Gtk_Backend.hh"
#include "Gtk_AreaFactory.hh"
#include "Gtk_MathGraphicDevice.hh"
#if GMV_ENABLE_BOXML
#include "Gtk_BoxGraphicDevice.hh"
#else
#include "BoxGraphicDevice.hh"
#endif // GMV_ENABLE_BOXML
#include "Gtk_PangoComputerModernShaper.hh"
#include "ShaperManager.hh"

Gtk_Backend::Gtk_Backend(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
  : Backend(l, conf)
{
  SmartPtr<Gtk_AreaFactory> factory = Gtk_AreaFactory::create();
  SmartPtr<Gtk_MathGraphicDevice> mgd = Gtk_MathGraphicDevice::create(l, conf);
  mgd->setFactory(factory);
  setMathGraphicDevice(mgd);
#if GMV_ENABLE_BOXML
  SmartPtr<Gtk_BoxGraphicDevice> bgd = Gtk_BoxGraphicDevice::create(l, conf);
  bgd->setFactory(factory);
  setBoxGraphicDevice(bgd);
#endif // GMV_ENABLE_BOXML

  SmartPtr<Gtk_PangoComputerModernShaper> pangoShaper = Gtk_PangoComputerModernShaper::create(l, conf);
  getShaperManager()->registerShaper(pangoShaper);
}

Gtk_Backend::~Gtk_Backend()
{ }

SmartPtr<Gtk_Backend>
Gtk_Backend::create(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
{ return new Gtk_Backend(l, conf); }
