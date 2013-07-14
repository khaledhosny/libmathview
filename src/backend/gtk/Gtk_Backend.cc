// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// Copyright (C) 2013, Khaled Hosny <khaledhosny@eglug.org>.
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

#include <gdk/gdk.h>
#include <pango/pangofc-font.h>
#include FT_TRUETYPE_TABLES_H

#include <map>

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "Gtk_Backend.hh"
#include "Gtk_AreaFactory.hh"
#include "Gtk_MathGraphicDevice.hh"
#include "Gtk_PangoShaper.hh"
#include "ShaperManager.hh"
#include "SpaceShaper.hh"

#define MATH_TAG  0X4D415448

Gtk_Backend::Gtk_Backend(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
  : Backend(l)
{
  SmartPtr<Gtk_AreaFactory> factory = Gtk_AreaFactory::create();
  SmartPtr<MathFont> mathfont = MathFont::create();

  String fontname = conf->getString(l, "default/font-family", DEFAULT_FONT_FAMILY);
  int fontsize = conf->getInt(l, "default/font-size", DEFAULT_FONT_SIZE);
  PangoFontDescription* description = pango_font_description_new();
  pango_font_description_set_family(description, fontname.c_str());
  pango_font_description_set_size(description, fontsize * PANGO_SCALE);

  GObjectPtr<PangoContext> context = gdk_pango_context_get();
  GObjectPtr<PangoFont> font = pango_context_load_font(context, description);
  FT_Byte *table = NULL;
  if (font)
    {
      PangoFcFont* fcfont = PANGO_FC_FONT(static_cast<PangoFont*>(font));
      FT_Face face = pango_fc_font_lock_face(fcfont);

      FT_ULong length = 0;
      FT_Error error = FT_Load_Sfnt_Table(face, MATH_TAG, 0, NULL, &length);
      if (!error)
        {
          table = new FT_Byte[length];
          error = FT_Load_Sfnt_Table(face, MATH_TAG, 0, table, &length);
          if (error)
            delete [] table;
          else
            mathfont->setData(table);
        }

      mathfont->setUnitsPerEM(face->units_per_EM);

      pango_fc_font_unlock_face(fcfont);
    }

  SmartPtr<Gtk_MathGraphicDevice> mgd = Gtk_MathGraphicDevice::create(l, mathfont);

  mgd->setFactory(factory);
  setMathGraphicDevice(mgd);

  getShaperManager()->registerShaper(Gtk_PangoShaper::create(context, font, mathfont));
  getShaperManager()->registerShaper(SpaceShaper::create());
}

Gtk_Backend::~Gtk_Backend()
{ }

SmartPtr<Gtk_Backend>
Gtk_Backend::create(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
{ return new Gtk_Backend(l, conf); }
