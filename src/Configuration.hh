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
// http://cs.unibo.it/~lpadovan/mml-widget/, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef Configuration_hh
#define Configuration_hh

#include <gdome.h>

#include "scaled.hh"
#include "String.hh"
#include "RGBValue.hh"
#include "Container.hh"

class Configuration {
public:
  Configuration(void);
  ~Configuration();

  bool Load(const char*);

  const Container<String*>& GetDictionaries(void) const { return dictionaries; }
  const Container<String*>& GetFonts(void) const { return fonts; }
  const Container<String*>& GetEntities(void) const { return entities; }
  const Container<String*>& GetT1ConfigFiles(void) const { return t1Configs; }

  bool     HasFontSize(void) const { return fontSizeSet; }
  unsigned GetFontSize(void) const { return HasFontSize() ? fontSize : DEFAULT_FONT_SIZE; }

  bool     HasColor(void) const { return colorSet; }
  RGBValue GetForeground(void) const { return HasColor() ? foreground : DEFAULT_FOREGROUND; }
  RGBValue GetBackground(void) const { return HasColor() ? background : DEFAULT_BACKGROUND; }

  bool     HasLinkColor(void) const { return linkColorSet; }
  RGBValue GetLinkForeground(void) const { return HasLinkColor() ? linkForeground : DEFAULT_LINK_FOREGROUND; }
  RGBValue GetLinkBackground(void) const { return HasLinkColor() ? linkBackground : DEFAULT_LINK_BACKGROUND; }

  bool     HasSelectColor(void) const { return selectColorSet; }
  RGBValue GetSelectForeground(void) const { return HasSelectColor() ? selectForeground : DEFAULT_SELECT_FOREGROUND; }
  RGBValue GetSelectBackground(void) const { return HasSelectColor() ? selectBackground : DEFAULT_SELECT_BACKGROUND; }

private:
  void ParseConfiguration(GdomeElement*);
  bool ParseColor(GdomeElement*, RGBValue&, RGBValue&);

  Container<String*> dictionaries;
  Container<String*> fonts;
  Container<String*> entities;
  Container<String*> t1Configs;

  bool     fontSizeSet;
  unsigned fontSize;

  bool     colorSet;
  RGBValue foreground;
  RGBValue background;

  bool     linkColorSet;
  RGBValue linkForeground;
  RGBValue linkBackground;

  bool     selectColorSet;
  RGBValue selectForeground;
  RGBValue selectBackground;
};

#endif // Configuration_hh
