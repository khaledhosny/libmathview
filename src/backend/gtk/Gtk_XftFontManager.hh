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

#ifndef __Gtk_XftFontManager_hh__
#define __Gtk_XftFontManager_hh__

#include <X11/Xft/Xft.h>

#include "Object.hh"
#include "String.hh"
#include "HashMap.hh"
#include "SmartPtr.hh"

class Gtk_XftFontManager : public Object
{
protected:
  Gtk_XftFontManager(void);
  virtual ~Gtk_XftFontManager();

public:
  static SmartPtr<Gtk_XftFontManager> create(void)
  { return new Gtk_XftFontManager(); }

  struct XLFD {
    XLFD(const String& v, const String& f, const String& w, const String& s, double sz, const String& c)
      : vendor(v), family(f), weight(w), slant(s), size(sz), charset(c)
    { }

    String vendor;
    String family;
    String weight;
    String slant;
    double size;
    String charset;

    String toString(void) const;
  };

  XftFont* getXftFont(const XLFD&) const;

private:
  XftFont* createXftFont(const XLFD&) const;

  typedef HASH_MAP_NS::hash_map<String,XftFont*,StringHash> XftFontCache;  
  mutable XftFontCache fontCache;
};

#endif // __Gtk_XftFontManager_hh__
