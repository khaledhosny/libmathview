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

#ifndef PS_T1_FontManager_hh
#define PS_T1_FontManager_hh

#ifdef HAVE_LIBT1

#include <list>

#include "T1_FontManager.hh"

class PS_T1_FontManager : public T1_FontManager
{
public:
  PS_T1_FontManager(void);
  virtual ~PS_T1_FontManager();

  void DumpFontDictionary(FILE*, bool = true, bool = true) const;
  void ResetUsedChars(void) const;

protected:
#if 0
  const char* GetFontFilePath(unsigned) const;
#endif
  virtual const class AFont* SearchNativeFont(const FontAttributes&,
                                              const ExtraFontAttributes*) const;

private:
  struct T1_FontDesc {
    unsigned id;
    char used[256];
  };

  static void SetUsedChars(std::list<T1_FontDesc*>&, unsigned);
  static void SetUsedChars(std::list<T1_FontDesc*>&, unsigned, const char[]);
};

#define TO_PS_T1_FONT_MANAGER(fm) (dynamic_cast<PS_T1_FontManager*>(fm))

#endif // HAVE_LIBT1

#endif // PS_T1_FontManager

