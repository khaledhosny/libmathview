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
