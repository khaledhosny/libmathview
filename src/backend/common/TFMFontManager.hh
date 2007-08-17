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

#ifndef __TFMFontManager_hh__
#define __TFMFontManager_hh__

#include "Object.hh"
#include "String.hh"
#include "StringHash.hh"
#include "HashMap.hh"
#include "SmartPtr.hh"
#include "scaled.hh"

class GMV_MathView_EXPORT TFMFontManager : public Object
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
