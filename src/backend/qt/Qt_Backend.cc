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

#include "Qt_Backend.hh"
#include "Qt_AreaFactory.hh"
#include "Qt_MathGraphicDevice.hh"
#include "Qt_Shaper.hh"
#include "ShaperManager.hh"
#include "SpaceShaper.hh"

Qt_Backend::Qt_Backend(QRawFont& rawFont)
{
    SmartPtr<Qt_AreaFactory> factory = Qt_AreaFactory::create();
    SmartPtr<MathFont> mathFont = MathFont::create();

    QByteArray table = rawFont.fontTable("MATH");
    if (table.size() > 0) {
        unsigned char* data = new unsigned char[table.size()+1];
        memcpy(data, (unsigned char*)table.constData(), table.size());
        mathFont->setData(data);
    } else {
        qDebug() << "no MATH table found in font" << rawFont.familyName();
    }
    mathFont->setUnitsPerEM(rawFont.unitsPerEm());

    SmartPtr<Qt_MathGraphicDevice> mgd = Qt_MathGraphicDevice::create(mathFont);

    mgd->setFactory(factory);
    setMathGraphicDevice(mgd);

    getShaperManager()->registerShaper(Qt_Shaper::create(rawFont, mathFont));
    getShaperManager()->registerShaper(SpaceShaper::create());
}

Qt_Backend::~Qt_Backend()
{
    getMathGraphicDevice()->clearCache();
}

SmartPtr<Qt_Backend>
Qt_Backend::create(QRawFont& rawFont)
{
    return new Qt_Backend(rawFont);
}
