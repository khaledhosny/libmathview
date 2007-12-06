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

#include <iostream>
#include <list>
#include "String.hh"
#include "FontDataBase.hh"

FontDataBase::FontDataBase()
{ }

FontDataBase::~FontDataBase()
{ }

SmartPtr<FontDataBase>
FontDataBase::create()
{ return new FontDataBase(); } 

int
FontDataBase::getFontId(const String& fontName, const float fontSize)
{
  for (std::list<FontDesc>::const_iterator p = fd.begin(); p != fd.end(); p++)
    if (p->name == fontName && p->size == fontSize)
      return p->id;
  
  const int id = fd.size();
  fd.push_back(FontDesc(fontName, fontSize, fd.size()));
  return id;
}

void
FontDataBase::dumpFontTable(std::ostream& os) const
{
  for (std::list<FontDesc>::const_iterator p = fd.begin(); p != fd.end(); p++)
  {
    os << "/F" << (p->id)
       << " /" << (p->name)
       << " findfont " << (p->size)
       << " scalefont " << "def" << std::endl;
  }
}

void
FontDataBase::recallFont(const int id, std::ostringstream& body)
{
  body << "F" << id << " setfont" << std::endl;
}

void 
FontDataBase::usedChar(const String& content, const String& family)
{ }

