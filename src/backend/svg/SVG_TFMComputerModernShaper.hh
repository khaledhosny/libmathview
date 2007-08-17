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

#ifndef __SVG_TFMComputerModernShaper_hh__
#define __SVG_TFMComputerModernShaper_hh__

#include "TFMComputerModernShaper.hh"

class SVG_TFMComputerModernShaper : public TFMComputerModernShaper
{
protected:
  SVG_TFMComputerModernShaper(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&);
  virtual ~SVG_TFMComputerModernShaper();

public:
  static SmartPtr<SVG_TFMComputerModernShaper> create(const SmartPtr<class AbstractLogger>&,
						      const SmartPtr<class Configuration>&);

protected:
  virtual AreaRef getGlyphArea(ComputerModernFamily::FontNameId,
			       ComputerModernFamily::FontSizeId, UChar8, int) const;
  virtual bool getGlyphData(const AreaRef&, SmartPtr<class TFMFont>&, UChar8&) const;
};

#endif // __SVG_TFMComputerModernShaper_hh__
