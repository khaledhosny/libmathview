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

#ifndef __Gtk_PangoFontManager_hh__
#define __Gtk_PangoFontManager_hh__

#include <pango/pangox.h>

#include "Object.hh"
#include "String.hh"
#include "StringHash.hh"
#include "HashMap.hh"
#include "SmartPtr.hh"

class Gtk_PangoFontManager : public Object
{
protected:
  Gtk_PangoFontManager(void);
  virtual ~Gtk_PangoFontManager();

public:
  static SmartPtr<Gtk_PangoFontManager> create(void)
  { return new Gtk_PangoFontManager(); }

  struct PangoFD {
    PangoFD(const String& f, PangoStyle s, PangoWeight w, int sz)
      : family(f), style(s), weight(w), size(sz)
    { }

    String family;
    PangoStyle style;
    PangoWeight weight;
    int size;

    PangoFontDescription* toPangoFontDescription(void) const;
  };

  struct XLFD {
    XLFD(const String& v, const String& f, const String& w, const String& s, int sz, const String& c)
      : vendor(v), family(f), weight(w), slant(s), size(sz), charset(c)
    { }

    String vendor;
    String family;
    String weight;
    String slant;
    int size;
    String charset;

    String toString(void) const;
  };

  PangoFont* getPangoFont(const PangoFD&, PangoXSubfont&) const;
  PangoFont* getPangoFont(const XLFD&, PangoXSubfont&) const;

private:
  PangoFont* createPangoFont(PangoFontDescription*, PangoXSubfont&) const;
  PangoFont* createPangoFont(const XLFD&, const String&, PangoXSubfont&) const;

  struct CachedPangoFontData
  {
    CachedPangoFontData(void) { }
    CachedPangoFontData(PangoFont* f, int s) : font(f), subfont(s) { }

    PangoFont* font;
    int subfont;
  };

  typedef HASH_MAP_NS::hash_map<String,CachedPangoFontData,StringHash> PangoFontCache;  
  mutable PangoFontCache fontCache;
};

#endif // __Gtk_PangoFontManager_hh__
