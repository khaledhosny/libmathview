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

#ifndef __FontDataBase_hh__
#define __FontDataBase_hh__

#include <iostream>
#include <list>
#include <sstream>
#include "Object.hh"
#include "SmartPtr.hh"
#include "String.hh"

class GMV_BackEnd_EXPORT FontDataBase : public Object
{
protected:
  FontDataBase();
  virtual ~FontDataBase();

public:
  virtual int getFontId(const String& fontName, float fontSize);
  virtual void dumpFontTable(std::ostream& os) const;
  virtual void recallFont(const int id, std::ostringstream& body);
  virtual void usedChar(const String& content, const String& family);
  static SmartPtr<FontDataBase> create();

private:
  struct FontDesc
  {
    String name;
    float  size;
    int    id;

    FontDesc(const String& fontName, const float fontSize, const int i)
      : name(fontName), size(fontSize), id(i)
    { };
  };
  
  std::list<FontDesc> fd; 
};

#endif  // __FontDataBase_hh__
