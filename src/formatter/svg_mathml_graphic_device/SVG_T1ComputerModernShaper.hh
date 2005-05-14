// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#ifndef __SVG_T1ComputerModernShaper_hh__
#define __SVG_T1ComputerModernShaper_hh__

#include "ComputerModernShaper.hh"

class SVG_T1ComputerModernShaper : public ComputerModernShaper
{
protected:
  SVG_T1ComputerModernShaper(void);
  virtual ~SVG_T1ComputerModernShaper();

public:
  static SmartPtr<SVG_T1ComputerModernShaper> create(void)
  { return new SVG_T1ComputerModernShaper(); }

  void setFontManager(const SmartPtr<class TFM_T1FontManager>&);

protected:
  SmartPtr<class SVG_T1Font> getT1Font(ComputerModernShaper::FontNameId, const scaled&) const;
  virtual AreaRef getGlyphArea(const SmartPtr<class AreaFactory>&,
			       ComputerModernShaper::FontNameId,
			       Char8, const scaled&) const;

private:
  SmartPtr<class TFM_T1FontManager> t1FontManager;
};

#endif // __SVG_T1ComputerModernShaper_hh__
