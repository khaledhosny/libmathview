// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __Gtk_T1ComputerModernShaper_hh__
#define __Gtk_T1ComputerModernShaper_hh__

#include "ComputerModernShaper.hh"

class Gtk_T1ComputerModernShaper : public ComputerModernShaper
{
protected:
  Gtk_T1ComputerModernShaper(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&);
  virtual ~Gtk_T1ComputerModernShaper();

public:
  static SmartPtr<Gtk_T1ComputerModernShaper> create(const SmartPtr<class AbstractLogger>&,
						     const SmartPtr<class Configuration>&);

  void setFontManager(const SmartPtr<class t1lib_T1FontManager>&);

protected:
  SmartPtr<class t1lib_T1Font> getT1Font(ComputerModernFamily::FontNameId,
					 ComputerModernFamily::FontSizeId, const scaled&) const;
  virtual AreaRef getGlyphArea(ComputerModernFamily::FontNameId,
			       ComputerModernFamily::FontSizeId, UChar8, int) const;

private:
  SmartPtr<class t1lib_T1FontManager> t1FontManager;
};

#endif // __Gtk_T1ComputerModernShaper_hh__
