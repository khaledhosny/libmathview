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

#ifndef __TFMFontManager_hh__
#define __TFMFontManager_hh__

#include "Object.hh"
#include "String.hh"
#include "HashMap.hh"
#include "SmartPtr.hh"
#include "scaled.hh"

class TFMFontManager : public Object
{
protected:
  TFMFontManager(const SmartPtr<class TFMManager>&);
  virtual ~TFMFontManager();

public:
  static SmartPtr<TFMFontManager> create(const SmartPtr<class TFMManager>&);

  SmartPtr<class TFMFont> getFont(const SmartPtr<class TFM>&, const scaled&) const;
  SmartPtr<class TFMFont> getFont(const String&, const scaled&) const;

protected:
  virtual SmartPtr<class TFMFont> createFont(const SmartPtr<class TFM>&, const scaled&) const;

private:
  struct CachedFontKey
  {
    CachedFontKey(const String& n, const scaled& sz)
      : name(n), size(sz) { }
    
    bool operator==(const CachedFontKey& key) const
    { return name == key.name && size == key.size; }
    
    String name;
    scaled size;
  };

  struct CachedFontHash
  {
    size_t operator()(const CachedFontKey& key) const
    { return StringHash()(key.name) ^ key.size.getValue(); }
  };

  typedef HASH_MAP_NS::hash_map<CachedFontKey,SmartPtr<class TFMFont>,CachedFontHash> FontCache;
  mutable FontCache fontCache;
  SmartPtr<class TFMManager> tfmManager;
};

#endif // __TFMFontManager_hh__
