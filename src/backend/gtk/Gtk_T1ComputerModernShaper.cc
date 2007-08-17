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

#include <config.h>

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "t1lib_T1Font.hh"
#include "Gtk_AreaFactory.hh"
#include "t1lib_T1FontManager.hh"
#include "Gtk_T1ComputerModernShaper.hh"
#include "Gtk_t1lib_T1GlyphArea.hh"

Gtk_T1ComputerModernShaper::Gtk_T1ComputerModernShaper(const SmartPtr<AbstractLogger>& l,
						       const SmartPtr<Configuration>& conf)
  : ComputerModernShaper(l, conf)
{
  setPostShapingMode(conf->getString(l, "gtk-backend/type1-computer-modern-shaper/post-shaping", "never"));
}

Gtk_T1ComputerModernShaper::~Gtk_T1ComputerModernShaper()
{ }

SmartPtr<Gtk_T1ComputerModernShaper>
Gtk_T1ComputerModernShaper::create(const SmartPtr<AbstractLogger>& l,
				   const SmartPtr<Configuration>& conf)
{ return new Gtk_T1ComputerModernShaper(l, conf); }

void
Gtk_T1ComputerModernShaper::setFontManager(const SmartPtr<t1lib_T1FontManager>& fm)
{
  assert(fm);
  t1FontManager = fm;
}

SmartPtr<t1lib_T1Font>
Gtk_T1ComputerModernShaper::getT1Font(ComputerModernFamily::FontNameId fontNameId,
				      ComputerModernFamily::FontSizeId designSize,
				      const scaled& size) const
{
  const String fontName = getFamily()->nameOfFont(fontNameId, designSize);
  if (SmartPtr<t1lib_T1Font> font = t1FontManager->getT1Font(fontName + ".pfb", size))
    return font;
  else
    return t1FontManager->getT1Font(fontName + ".pfa", size);
}

AreaRef
Gtk_T1ComputerModernShaper::getGlyphArea(ComputerModernFamily::FontNameId fontNameId,
					 ComputerModernFamily::FontSizeId designSize,
					 UChar8 index, int size) const
{

  assert(t1FontManager);
  const SmartPtr<t1lib_T1Font> font = getT1Font(fontNameId, designSize, scaled(size));
  assert(font);

#if 0
  std::vector<AreaRef> c;
  c.reserve(256);
  for (int i = 0; i < 256; i++)
    c.push_back(Gtk_t1lib_T1GlyphArea::create(font, i));

  return factory->horizontalArray(c);
#endif
  AreaRef res = Gtk_t1lib_T1GlyphArea::create(font, index);
  return res;
}
