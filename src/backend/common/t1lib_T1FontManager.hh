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

#ifndef __t1lib_T1FontManager_hh__
#define __t1lib_T1FontManager_hh__

#include "Object.hh"
#include "String.hh"
#include "HashMap.hh"
#include "SmartPtr.hh"
#include "scaled.hh"

class GMV_MathView_EXPORT t1lib_T1FontManager : public Object
{
protected:
  t1lib_T1FontManager(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&);
  virtual ~t1lib_T1FontManager();

public:
  static SmartPtr<t1lib_T1FontManager> create(const SmartPtr<class AbstractLogger>&, 
					      const SmartPtr<class Configuration>&);

  String getFontFileName(int) const;
  SmartPtr<class t1lib_T1Font> getT1Font(const String&, const scaled&) const;

protected:
  int loadFont(const String&) const;
  int getFontId(const String&) const;
  virtual SmartPtr<class t1lib_T1Font> createT1Font(const String&, const scaled&) const;

private:
  static bool firstTime;

  struct CachedT1FontKey
  {
    CachedT1FontKey(const String& n, const scaled& sz)
      : name(n), size(sz) { }
    
    bool operator==(const CachedT1FontKey& key) const
    { return name == key.name && size == key.size; }
    
    String name;
    scaled size;
  };

  struct CachedT1FontHash
  {
    size_t operator()(const CachedT1FontKey& key) const
    { return StringHash()(key.name) ^ key.size.getValue(); }
  };

  typedef HASH_MAP_NS::hash_map<CachedT1FontKey,SmartPtr<class t1lib_T1Font>,CachedT1FontHash> T1FontCache;
  mutable T1FontCache fontCache;
};

#endif // __t1lib_T1FontManager_hh__
