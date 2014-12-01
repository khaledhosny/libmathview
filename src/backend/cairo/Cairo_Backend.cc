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

#if HAVE_CAIRO_SCALED_FONT_GET_TABLE
#include <cairo.h>
#include <hb-ot.h>
#else
#include <cairo-ft.h>
#include <hb-ft.h>
#endif

#include <map>

#include "AreaFactory.hh"
#include "Cairo_Backend.hh"
#include "Cairo_Shaper.hh"
#include "MathGraphicDevice.hh"
#include "ShaperManager.hh"
#include "SpaceShaper.hh"

#if HAVE_CAIRO_SCALED_FONT_GET_TABLE
static hb_blob_t *
reference_table(hb_face_t *, hb_tag_t tag, void *user_data)
{
  cairo_scaled_font_t* font = reinterpret_cast<cairo_scaled_font_t*>(user_data);

  unsigned long length = 0;
  cairo_status_t status = cairo_scaled_font_get_table(font, tag, 0, NULL, &length);

  if (status || length <= 0)
    return NULL;

  unsigned char* table = new unsigned char[length];
  cairo_scaled_font_get_table(font, tag, 0, table, &length);

  return hb_blob_create((const char*)table, length, HB_MEMORY_MODE_DUPLICATE, NULL, NULL);
}
#endif

Cairo_Backend::Cairo_Backend(cairo_scaled_font_t* font)
{
  SmartPtr<AreaFactory> factory = AreaFactory::create();

#if HAVE_CAIRO_SCALED_FONT_GET_TABLE
  hb_face_t *hb_face = hb_face_create_for_tables(reference_table, (void*)(font), NULL);
  hb_face_set_index(hb_face, 0);
  //hb_face_set_upem(hb_face, 0);
  hb_font_t *hb_font = hb_font_create(hb_face);
  hb_face_destroy(hb_face);

  hb_ot_font_set_funcs(hb_font);
#else
  FT_Face face = cairo_ft_scaled_font_lock_face(const_cast<cairo_scaled_font_t*>(font));
  hb_font_t *hb_font = hb_ft_font_create(face, NULL);

  cairo_ft_scaled_font_unlock_face(font);
#endif

  SmartPtr<MathGraphicDevice> mgd = MathGraphicDevice::create(hb_font);

  mgd->setFactory(factory);
  setMathGraphicDevice(mgd);

  getShaperManager()->registerShaper(Cairo_Shaper::create(font, hb_font));
  getShaperManager()->registerShaper(SpaceShaper::create());
}

Cairo_Backend::~Cairo_Backend()
{ }

SmartPtr<Cairo_Backend>
Cairo_Backend::create(cairo_scaled_font_t* font)
{ return new Cairo_Backend(font); }
