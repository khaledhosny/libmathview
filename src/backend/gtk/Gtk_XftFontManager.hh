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

#ifndef __Gtk_XftFontManager_hh__
#define __Gtk_XftFontManager_hh__

#include <X11/Xft/Xft.h>

#include "Object.hh"
#include "String.hh"
#include <tr1/unordered_map>
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

  typedef std::tr1::unordered_map<String,XftFont*,StringHash> XftFontCache;  
  mutable XftFontCache fontCache;
};

#endif // __Gtk_XftFontManager_hh__
