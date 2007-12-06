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

#ifndef __Gtk_AdobeShaper_hh__
#define __Gtk_AdobeShaper_hh__

#include "Shaper.hh"

class Gtk_AdobeShaper : public Shaper
{
protected:
  Gtk_AdobeShaper(void);
  virtual ~Gtk_AdobeShaper();

public:
  enum { N_FONTS = 10 };

  static SmartPtr<Gtk_AdobeShaper> create(void);

  void setFontManager(const SmartPtr<class Gtk_PangoFontManager>&);
#if HAVE_XFT
  void setFontManager(const SmartPtr<class Gtk_XftFontManager>&);
#endif // HAVE_XFT

  virtual void registerShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual void unregisterShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual void shape(class ShapingContext&) const;

protected:
  AreaRef getGlyphArea(const SmartPtr<class Gtk_AreaFactory>&, unsigned, unsigned, const scaled&) const;
  AreaRef createPangoGlyphArea(const SmartPtr<class Gtk_AreaFactory>&, unsigned, unsigned, const scaled&) const;
#if HAVE_XFT
  AreaRef createXftGlyphArea(const SmartPtr<class Gtk_AreaFactory>&, unsigned, unsigned, const scaled&) const;
#endif // HAVE_XFT
  AreaRef shapeChar(const class ShapingContext&) const;
  AreaRef shapeStretchyCharV(const class ShapingContext&) const;
  AreaRef shapeStretchyCharH(const class ShapingContext&) const;

  SmartPtr<class Gtk_PangoFontManager> pangoFontManager;
#if HAVE_XFT
  SmartPtr<class Gtk_XftFontManager> xftFontManager;
#endif // HAVE_XFT
};

#endif // __Gtk_AdobeShaper_hh__
