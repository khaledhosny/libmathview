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

#ifndef T1_FontManager_hh
#define T1_FontManager_hh

#ifdef HAVE_LIBT1

#include "FontManager.hh"
#include "FontAttributes.hh"

class T1_FontManager : public FontManager {
public:
  T1_FontManager(void);
  virtual ~T1_FontManager();

  virtual bool IsAvailable(const FontAttributes&, const ExtraFontAttributes* = 0) const;

protected:
  int SearchNativeFontAux(const FontAttributes&,
                          const ExtraFontAttributes*,
                          float&) const;

  virtual const class AFont* SearchNativeFont(const FontAttributes&,
					      const ExtraFontAttributes*) const;

private:
  int SearchT1FontId(const char*) const;

  static bool firstTime;
};

#define TO_T1_FONT_MANAGER(ptr) dynamic_cast<T1_FontManager*>(ptr)

#endif // HAVE_LIBT1

#endif // T1_FontManager_hh
