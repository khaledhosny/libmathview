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

#ifndef FontMapper_hh
#define FontMapper_hh

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stddef.h>

#include "Char.hh"
#include "AFont.hh"
#include "FontAttributes.hh"

typedef unsigned (*CharMapping)(Char);

class FontMapper {
public:
  FontMapper(void);
  ~FontMapper();
  
  void Reset(void);
  void Configure(const char*, Char, Char, const char*, const char*, CharMapping);
  const AFont* SearchNearFont(const FontAttributes&, const char* = NULL, const char* = NULL) const;
  const AFont* Map(const FontAttributes&, Char, CharMapping*) const;
  bool SameFont(Char) const;

private:
  GdkFont* SearchGdkFont(const char*, const char*, char, int, const char*) const;

  struct FontMap {
    const char*    name;
    Char           start;
    Char           end;
    const char*    fontFamily; // if != NULL overrides font family
    const char*    charSet;    // if == NULL then * is used
    CharMapping    map;

    const FontMap* next;
  };   

  const FontMap* first;
  mutable const FontMap* lastFound;
};

#endif
