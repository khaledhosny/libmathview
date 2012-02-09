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

#ifndef __Gtk_PangoFontManager_hh__
#define __Gtk_PangoFontManager_hh__

#include <pango/pangox.h>

#include "Object.hh"
#include "String.hh"
#include "StringHash.hh"
#include <tr1/unordered_map>
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

  typedef std::tr1::unordered_map<String,CachedPangoFontData,StringHash> PangoFontCache;  
  mutable PangoFontCache fontCache;
};

#endif // __Gtk_PangoFontManager_hh__
