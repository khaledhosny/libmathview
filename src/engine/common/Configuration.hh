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

#ifndef __Configuration_hh__
#define __Configuration_hh__

#include <vector>

#include "defs.h"
#include "scaled.hh"
#include "String.hh"
#include "RGBColor.hh"

class Configuration
{
public:
  Configuration(void)
    : drawMissingGlyphs(true),
      fontSize(DEFAULT_FONT_SIZE), 
      foreground(DEFAULT_FOREGROUND),
      background(DEFAULT_BACKGROUND),
      linkForeground(DEFAULT_LINK_FOREGROUND),
      linkBackground(DEFAULT_LINK_BACKGROUND),
      selectForeground(DEFAULT_SELECT_FOREGROUND),
      selectBackground(DEFAULT_SELECT_BACKGROUND)
  { }

  ~Configuration() { }

  void addDictionary(const String& s) { dictionaries.push_back(s); }
  const std::vector<String>& getDictionaries(void) const { return dictionaries; }

  bool getDrawMissingGlyphs(void) const { return drawMissingGlyphs; }
  void setDrawMissingGlyphs(bool b) { drawMissingGlyphs = b; }

  void setFontSize(unsigned s) { fontSize = s; }
  unsigned getFontSize(void) const { return fontSize; }

  void setForeground(const RGBColor& c) { foreground = c; }
  void setBackground(const RGBColor& c) { background = c; }
  RGBColor getForeground(void) const { return foreground; }
  RGBColor getBackground(void) const { return background; }

  void setLinkForeground(const RGBColor& c) { linkForeground = c; }
  void setLinkBackground(const RGBColor& c) { linkBackground = c; }
  RGBColor getLinkForeground(void) const { return linkForeground; }
  RGBColor getLinkBackground(void) const { return linkBackground; }

  void setSelectForeground(const RGBColor& c) { selectForeground = c; }
  void setSelectBackground(const RGBColor& c) { selectBackground = c; }
  RGBColor getSelectForeground(void) const { return selectForeground; }
  RGBColor getSelectBackground(void) const { return selectBackground; }

private:
  std::vector<std::string> dictionaries;

  bool drawMissingGlyphs;
  int verbosity;
  unsigned fontSize;

  RGBColor foreground;
  RGBColor background;

  RGBColor linkForeground;
  RGBColor linkBackground;

  RGBColor selectForeground;
  RGBColor selectBackground;
};

#endif // __Configuration_hh__
