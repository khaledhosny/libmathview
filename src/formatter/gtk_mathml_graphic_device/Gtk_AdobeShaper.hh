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

#include <functional>

#include <pango/pangox.h>
#include <X11/Xft/Xft.h>

#include "HashMap.hh"
#include "Shaper.hh"

class Gtk_AdobeShaper : public Shaper
{
protected:
  Gtk_AdobeShaper(void) { }
  virtual ~Gtk_AdobeShaper() { }

public:
  static SmartPtr<Gtk_AdobeShaper> create(void)
  { return new Gtk_AdobeShaper(); }

  virtual void registerShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual void unregisterShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual unsigned shape(const class MathFormattingContext&, class ShapingResult&) const;

protected:
  static const char* getXLFD(unsigned, const scaled&);

  PangoFont* getPangoFont(unsigned, const scaled&, PangoXSubfont&) const;
  XftFont* getXftFont(unsigned, const scaled&) const;
  void getGlyphExtents(PangoFont*, PangoGlyphString*, PangoRectangle*) const;
  bool shapeChar(const class MathFormattingContext&, class ShapingResult&, const class GlyphSpec&) const;
  bool shapeStretchyCharV(const class MathFormattingContext&, class ShapingResult&, const class GlyphSpec&) const;
  bool shapeStretchyCharH(const class MathFormattingContext&, class ShapingResult&, const class GlyphSpec&) const;

  struct CachedFontKey
  {
    CachedFontKey(unsigned i, const scaled& s) : index(i), size(s) { }

    unsigned index;
    scaled size;

    bool operator==(const CachedFontKey& k) const
    { return index == k.index && size == k.size; }
  };

  struct CachedFontKeyHash
  {
    size_t operator()(const CachedFontKey& key) const
    { return key.index + key.size.getValue(); }
  };

  struct CachedPangoFontData
  {
    CachedPangoFontData(void) { }
    CachedPangoFontData(PangoFont* f, int s) : font(f), subfont(s) { }

    PangoFont* font;
    int subfont;
  };

  struct CachedAreaKey
  {
    CachedAreaKey(DOM::Char32 c, const scaled& s) : ch(c), size(s) { }

    DOM::Char32 ch;
    scaled size;

    bool operator==(const CachedAreaKey& k) const
    { return ch == k.ch && size == k.size; }
  };

  struct CachedAreaKeyHash
  {
    bool operator()(const CachedAreaKey& key) const
    { return key.ch + key.size.getValue(); }
  };

  typedef HASH_MAP_NS::hash_map<CachedFontKey,CachedPangoFontData,CachedFontKeyHash> PangoFontCache;
  typedef HASH_MAP_NS::hash_map<CachedAreaKey,AreaRef,CachedAreaKeyHash> AreaCache;

  mutable PangoFontCache pangoFontCache;
  mutable AreaCache areaCache;
};

#endif // __Gtk_AdobeShaper_hh__
