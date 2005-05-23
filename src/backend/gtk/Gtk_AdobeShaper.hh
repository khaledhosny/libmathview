// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

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
