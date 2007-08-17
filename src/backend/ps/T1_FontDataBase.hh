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

#ifndef __T1_FontDataBase_hh__
#define __T1_FontDataBase_hh__

#include <t1lib.h>
#include <config.h>
#include <map>
#include <list>

#include "Object.hh"
#include "SmartPtr.hh"

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "FontDataBase.hh"

class GMV_BackEnd_EXPORT T1_FontDataBase : public FontDataBase
{
protected:
  T1_FontDataBase(const SmartPtr<AbstractLogger>&, const SmartPtr<class Configuration>&, bool);
  virtual ~T1_FontDataBase();

public:
  static SmartPtr<T1_FontDataBase> create(const SmartPtr<AbstractLogger>&, const SmartPtr<class Configuration>&, bool);			         
  virtual int getFontId(const String& fontName, float fontSize);
  virtual void dumpFontTable(std::ostream& os) const;
  virtual void recallFont(const int id, std::ostringstream& body);
  virtual void usedChar(const String& content, const String& family);

private:
  SmartPtr<class AbstractLogger> logger;
  const bool subset;

  struct T1_DataBase 
  {
    String fontName;
    UChar8 used[256];
    int id;

    T1_DataBase(String& name, int i, int initVal);
  };

  std::list<T1_DataBase> T1_dB;
};

#endif // __T1_FontDataBase_hh__
