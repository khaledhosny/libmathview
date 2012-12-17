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

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "Gtk_PangoLayoutLineArea.hh"
#include "Gtk_PangoComputerModernShaper.hh"
#include "Gtk_DefaultPangoShaper.hh"

Gtk_PangoComputerModernShaper::Gtk_PangoComputerModernShaper(const SmartPtr<AbstractLogger>& l,
							     const SmartPtr<Configuration>& conf)
  : ComputerModernShaper(l, conf)
{
  GObjectPtr<PangoContext> context = gdk_pango_context_get();
  pangoShaper = Gtk_DefaultPangoShaper::create(l, conf);
  pangoShaper->setPangoContext(context);
  setPostShapingMode(conf->getString(l, "gtk-backend/pango-computer-modern-shaper/post-shaping", "never"));
}

Gtk_PangoComputerModernShaper::~Gtk_PangoComputerModernShaper()
{ }

SmartPtr<Gtk_PangoComputerModernShaper>
Gtk_PangoComputerModernShaper::create(const SmartPtr<AbstractLogger>& l,
				      const SmartPtr<Configuration>& conf)
{ return new Gtk_PangoComputerModernShaper(l, conf); }

AreaRef
Gtk_PangoComputerModernShaper::getGlyphArea(ComputerModernFamily::FontNameId fontNameId,
					    ComputerModernFamily::FontSizeId designSize,
					    UChar8 index, int size) const
{
  Gtk_DefaultPangoShaper::PangoTextAttributes attributes;
  attributes.variant = NORMAL_VARIANT;
  attributes.family = ComputerModernFamily::nameOfFont(fontNameId, designSize);
  attributes.style = PANGO_STYLE_NORMAL;
  attributes.weight = PANGO_WEIGHT_NORMAL;

  gchar buffer[6];
  gint length = g_unichar_to_utf8(toTTFGlyphIndex(ComputerModernFamily::encIdOfFontNameId(fontNameId), index), buffer);
  PangoLayout* layout = pangoShaper->createPangoLayout(buffer, length, size, attributes);
  return Gtk_PangoLayoutLineArea::create(layout);
}
