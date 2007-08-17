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

