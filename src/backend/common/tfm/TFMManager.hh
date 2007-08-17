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

#ifndef __TFMManager_hh__
#define __TFMManager_hh__

#include "HashMap.hh"
#include "Object.hh"
#include "SmartPtr.hh"
#include "String.hh"
#include "StringHash.hh"
#include "TFM.hh"
#include "scaled.hh"

class TFMManager : public Object
{
protected:
  TFMManager(void);
  virtual ~TFMManager();

public:
  static SmartPtr<TFMManager> create(void)
  { return new TFMManager(); }

  SmartPtr<class TFM> getTFM(const String&) const;

protected:
  typedef void (*TFMTables)(TFM::Font*&, TFM::Dimension*&, TFM::Character*&);

private:
  typedef HASH_MAP_NS::hash_map<String,SmartPtr<class TFM>,StringHash> TFMCache;
  mutable TFMCache tfmCache;  

  struct TFMTable
  {
    const char* name;
    TFMTables tables;
  };
};

#endif // __TFMManager_hh__
