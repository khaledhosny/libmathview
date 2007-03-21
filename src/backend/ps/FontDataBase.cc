// Copyright (C) 2000-2006, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a Gtk widget for MathML.
// 
// GtkMathView is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// GtkMathView is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GtkMathView; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// For details, see the GtkMathView World-Wide-Web page,
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <padovani@sti.uniurb.it>

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

