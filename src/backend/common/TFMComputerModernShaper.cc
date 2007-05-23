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

#include <config.h>

#include "ShapingContext.hh"
#include "TFM.hh"
#include "TFMFont.hh"
#include "TFMFontManager.hh"
#include "TFMComputerModernShaper.hh"
#include "AreaFactory.hh"

TFMComputerModernShaper::TFMComputerModernShaper(const SmartPtr<AbstractLogger>& l,
						 const SmartPtr<Configuration>& conf)
  : ComputerModernShaper(l, conf)
{ }

TFMComputerModernShaper::~TFMComputerModernShaper()
{ }

void
TFMComputerModernShaper::setFontManager(const SmartPtr<TFMFontManager>& fm)
{
  assert(fm);
  tfmFontManager = fm;
}

SmartPtr<TFMFontManager>
TFMComputerModernShaper::getFontManager() const
{ return tfmFontManager; }

ComputerModernFamily::FontNameId
TFMComputerModernShaper::fontNameIdOfTFM(const SmartPtr<TFM>& tfm)
{
  assert(tfm);
  static const char* name[] = {
    "CMR",
    "CMB",
    "CMBX",
    "CMBXTI",
    "CMTI",
    "CMSS",
    "CMSSI",
    "CMSSBX",
    "CMTT",
    "CMSY",
    "CMBSY",
    "CMMI",
    "CMMIB",
    "CMEX",
    "MSAM",
    "MSBM"
  };
  assert(sizeof(name) / sizeof(const char*) == ComputerModernFamily::FN_NOT_VALID);
  const String familyName = tfm->getFamily();
  for (int i = 0; i < sizeof(name) / sizeof(const char*); i++)
    if (familyName == name[i])
      return ComputerModernFamily::FontNameId(i);
  return ComputerModernFamily::FN_NIL;
}

void
TFMComputerModernShaper::postShape(ShapingContext& context) const
{
  if (context.nAreas() > 1)
    {
      UChar8 index1;
      UChar8 index2;
      SmartPtr<TFMFont> font1;
      SmartPtr<TFMFont> font2;
      if (getGlyphData(context.getArea(-2), font1, index1) && getGlyphData(context.getArea(-1), font2, index2))
	{
	  if (font1 == font2)
	    {
	      const SmartPtr<TFM> tfm = font1->getTFM();
	      UChar8 newGlyph;
	      UChar8 ligatureMode;
	      scaled kerning;
	      if (tfm->getGlyphLigature(index1, index2, newGlyph, ligatureMode))
		{
		  CharIndex n1;
		  CharIndex n2;
		  const AreaRef a2 = context.popArea(n2);
		  const AreaRef a1 = context.popArea(n1);
		  switch (ligatureMode)
		    {
		    default:
		      {
			AreaRef newArea = getGlyphArea(fontNameIdOfTFM(tfm), 
						       ComputerModernFamily::fontSizeIdOfSize(tfm->getDesignSize().toInt()),
						       newGlyph, font1->getSize().toInt());
			context.pushArea(n1 + n2, newArea);
		      }
		    }
		}
	      else if (tfm->getGlyphKerning(index1, index2, kerning))
		{
		  CharIndex n2;
		  const AreaRef a2 = context.popArea(n2);
		  context.pushArea(0, context.getFactory()->horizontalSpace(kerning * font1->getScale()));
		  context.pushArea(n2, a2);
		}
	    }
	}
    }
}

SmartPtr<TFMFont>
TFMComputerModernShaper::getFont(ComputerModernFamily::FontNameId fontNameId,
				 ComputerModernFamily::FontSizeId designSize, const scaled& size) const
{
  assert(tfmFontManager);
  return tfmFontManager->getFont(ComputerModernFamily::nameOfFont(fontNameId, designSize), size);
}

void
TFMComputerModernShaper::computeCombiningCharOffsets(const AreaRef& base,
						     const AreaRef& accent,
						     const AreaRef& ex,
						     scaled& dx,
						     scaled& dy) const
{
  ComputerModernShaper::computeCombiningCharOffsets(base, accent, ex, dx, dy);

  UChar8 index;
  UChar8 baseIndex;
  SmartPtr<TFMFont> font;
  SmartPtr<TFMFont> accentFont;
  SmartPtr<TFMFont> baseFont;

  if (getGlyphData(accent, accentFont, index))
    {
      const SmartPtr<TFM> accentTFM  = accentFont->getTFM();
      ComputerModernFamily::FontNameId accentNameId = fontNameIdOfTFM(accentTFM);
  
      if (accentNameId == ComputerModernFamily::FN_CMBXTI ||
	  accentNameId == ComputerModernFamily::FN_CMTI ||
	  accentNameId == ComputerModernFamily::FN_CMMI ||
	  accentNameId == ComputerModernFamily::FN_CMSSI)
	{
	  return;
	}
    } 

  if (getGlyphData(base, baseFont, baseIndex))
    {
      const SmartPtr<TFM> tfm = baseFont->getTFM();

      ComputerModernFamily::FontNameId fontNameId = fontNameIdOfTFM(tfm);
      ComputerModernFamily::FontEncId fontEncId = ComputerModernFamily::encIdOfFontNameId(fontNameId);

      if (fontEncId == ComputerModernFamily::FE_CMMI || fontEncId == ComputerModernFamily::FE_CMR)
	{
	  const Char8 skewChar = (fontEncId == ComputerModernFamily::FE_CMMI) ? 0177 : 060;
	  scaled kerning;

	  if (tfm->getGlyphKerning(baseIndex, skewChar, kerning))
	    {
	      kerning *= baseFont->getScale();
	      dx += kerning;
	    }
	}
    }
}
