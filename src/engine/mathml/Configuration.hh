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

#include <vector>

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "defs.h"
#include "scaled.hh"
#include "String.hh"
#include "RGBColor.hh"

class Configuration
{
public:
  Configuration(void);
  ~Configuration();

  bool Load(const char*);

  const std::vector<String>& GetDictionaries(void) const { return dictionaries; }
  const std::vector<String>& GetEntities(void) const { return entities; }
  const std::vector<String>& GetT1ConfigFiles(void) const { return t1Configs; }

  bool     HasFontSize(void) const { return fontSizeSet; }
  unsigned GetFontSize(void) const { return HasFontSize() ? fontSize : DEFAULT_FONT_SIZE; }

  bool     HasColor(void) const { return colorSet; }
  RGBColor GetForeground(void) const { return HasColor() ? foreground : DEFAULT_FOREGROUND; }
  RGBColor GetBackground(void) const { return HasColor() ? background : DEFAULT_BACKGROUND; }

  bool     HasLinkColor(void) const { return linkColorSet; }
  bool     HasTransparentLinkBackground(void) const { return transparentLinkBackground; }
  RGBColor GetLinkForeground(void) const { return HasLinkColor() ? linkForeground : DEFAULT_LINK_FOREGROUND; }
  RGBColor GetLinkBackground(void) const { return HasLinkColor() ? linkBackground : DEFAULT_LINK_BACKGROUND; }

  bool     HasSelectColor(void) const { return selectColorSet; }
  RGBColor GetSelectForeground(void) const { return HasSelectColor() ? selectForeground : DEFAULT_SELECT_FOREGROUND; }
  RGBColor GetSelectBackground(void) const { return HasSelectColor() ? selectBackground : DEFAULT_SELECT_BACKGROUND; }

private:
#if defined(HAVE_GMETADOM)
  void ParseConfiguration(const DOM::Element&);
  bool ParseColor(const DOM::Element&, RGBColor&, RGBColor&);
  bool ParseColor(const DOM::Element&, RGBColor&, RGBColor&, bool&);
#endif // HAVE_GMETADOM

  std::vector<std::string> dictionaries;
  std::vector<std::string> entities;
  std::vector<std::string> t1Configs;

  bool     fontSizeSet;
  unsigned fontSize;

  bool     colorSet;
  RGBColor foreground;
  RGBColor background;

  bool     linkColorSet;
  RGBColor linkForeground;
  RGBColor linkBackground;
  bool     transparentLinkBackground;

  bool     selectColorSet;
  RGBColor selectForeground;
  RGBColor selectBackground;
};

#endif // Configuration_hh
