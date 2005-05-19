// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
