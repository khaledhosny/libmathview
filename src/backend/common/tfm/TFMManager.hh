// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// <lpadovan@cs.unibo.it>

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
