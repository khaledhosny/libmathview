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

#ifndef __TFMComputerModernShaper_hh__
#define __TFMComputerModernShaper_hh__

#include "ComputerModernShaper.hh"

class GMV_MathView_EXPORT TFMComputerModernShaper : public ComputerModernShaper
{
protected:
  TFMComputerModernShaper(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&);
  virtual ~TFMComputerModernShaper();

public:
  void setFontManager(const SmartPtr<class TFMFontManager>&);
  SmartPtr<class TFMFontManager> getFontManager(void) const;

protected:
  static ComputerModernFamily::FontNameId fontNameIdOfTFM(const SmartPtr<class TFM>&);
  virtual void postShape(class ShapingContext&) const;
  virtual SmartPtr<class TFMFont> getFont(ComputerModernFamily::FontNameId,
					  ComputerModernFamily::FontSizeId, const scaled&) const;
  virtual bool getGlyphData(const AreaRef&, SmartPtr<class TFMFont>&, UChar8&) const = 0;
  virtual bool computeCombiningCharOffsetsAbove(const AreaRef&, const AreaRef&,
					        scaled&, scaled&) const;
  
private:
  SmartPtr<class TFMFontManager> tfmFontManager;
};


#endif // __TFMComputerModernShaper_hh__
