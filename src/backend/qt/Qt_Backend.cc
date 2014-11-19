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

#include <map>
#include <cstring>
#include <QRawFont>
#include <QByteArray>
#include <QDebug>
#include <hb-ot.h>

#include "AreaFactory.hh"
#include "MathGraphicDevice.hh"
#include "Qt_Backend.hh"
#include "Qt_Shaper.hh"
#include "ShaperManager.hh"
#include "SpaceShaper.hh"

static hb_blob_t *
reference_table(hb_face_t *, hb_tag_t tag, void *user_data)
{
  QRawFont *rawFont = reinterpret_cast<QRawFont*>(user_data);
  char tagstr[5] = {0};
  hb_tag_to_string(tag, tagstr);

  QByteArray table = rawFont->fontTable(tagstr);
  if (table.isEmpty())
    return NULL;

  return hb_blob_create(table.constData(), table.size(), HB_MEMORY_MODE_DUPLICATE, NULL, NULL);
}

Qt_Backend::Qt_Backend(const QRawFont& rawFont)
{
    SmartPtr<AreaFactory> factory = AreaFactory::create();

    hb_face_t *hb_face = hb_face_create_for_tables(reference_table, (void*)(&rawFont), NULL);
    hb_face_set_index(hb_face, 0);
    hb_face_set_upem(hb_face, rawFont.unitsPerEm());
    hb_font_t *hb_font = hb_font_create(hb_face);
    hb_face_destroy(hb_face);

    hb_ot_font_set_funcs(hb_font);

    SmartPtr<MathGraphicDevice> mgd = MathGraphicDevice::create(hb_font);

    mgd->setFactory(factory);
    setMathGraphicDevice(mgd);

    getShaperManager()->registerShaper(Qt_Shaper::create(rawFont, hb_font));
    getShaperManager()->registerShaper(SpaceShaper::create());
}

Qt_Backend::~Qt_Backend()
{
    getMathGraphicDevice()->clearCache();
}

SmartPtr<Qt_Backend>
Qt_Backend::create(const QRawFont& rawFont)
{
    return new Qt_Backend(rawFont);
}
