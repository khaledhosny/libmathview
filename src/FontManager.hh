// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef FontManager_hh
#define FontManager_hh

#include "Container.hh"
#include "FontAttributes.hh"

class FontManager {
public:
  FontManager(void);
  virtual ~FontManager();

  virtual bool IsAvailable(const FontAttributes&, const class ExtraFontAttributes* = 0) const = 0;
  const class AFont* GetFont(const FontAttributes&, const class ExtraFontAttributes* = 0) const;

  void ResetUsedFonts(void) const;
  void MarkAsUsed(const class AFont*) const;

protected:
  struct Bucket {
    FontAttributes fontAttributes;
    const class ExtraFontAttributes* extraFontAttributes;
    const class AFont* font;
    bool used; // if true then this font is actually used by someone
  };

  Bucket* SearchFont(const FontAttributes&, const class ExtraFontAttributes*) const;
  Bucket* SearchFont(const class AFont*) const;
  virtual const class AFont* SearchNativeFont(const FontAttributes&,
					      const class ExtraFontAttributes*) const = 0;

  mutable Container<Bucket*> content;
};

#endif // FontManager_hh
