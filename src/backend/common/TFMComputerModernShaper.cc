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

#include "ShapingContext.hh"
#include "TFM.hh"
#include "TFMFont.hh"
#include "TFMFontManager.hh"
#include "TFMComputerModernShaper.hh"
#include "AreaFactory.hh"
#include "GlyphArea.hh"

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
  return tfmFontManager->getFont(ComputerModernFamily::nameOfFont(fontNameId, designSize),
				 size);
}

bool
TFMComputerModernShaper::computeCombiningCharOffsetsAbove(const AreaRef& base,
						          const AreaRef& script,
						     	  scaled& dx,
						          scaled& dy) const
{
  AreaRef baseArea = base->getGlyphArea();
  AreaRef scriptArea = script->getGlyphArea();

  UChar8 accentIndex;
  UChar8 baseIndex;
  SmartPtr<TFMFont> font;
  SmartPtr<TFMFont> accentFont;
  SmartPtr<TFMFont> baseFont;
 
  bool res;

  if (baseArea && scriptArea)
    {
      dx = (base->box().width - script->box().width) / 2;

      if (!getGlyphData(baseArea, baseFont, baseIndex))
	return false;

      const bool accentData = getGlyphData(scriptArea, accentFont, accentIndex);
      scaled x_height;
      SmartPtr<TFM> accentTFM;
      ComputerModernFamily::FontNameId accentNameId;

      if (accentData)
	{
	  accentTFM = accentFont->getTFM();
	  accentNameId = fontNameIdOfTFM(accentTFM);
      
	  ComputerModernFamily::FontSizeId designAccentSize;
	  scaled accentSize = accentFont->getSize();
      
	  family->findFont(NORMAL_VARIANT,
			   ComputerModernFamily::encIdOfFontNameId(accentNameId),
			   accentSize,
			   designAccentSize);
      
	  SmartPtr<TFMFont> exTFMFont = getFont(accentNameId,
						//ComputerModernFamily::FN_CMR,
						designAccentSize,
						accentFont->getSize());

	  const unsigned index = 5;  //position of x-height
	  const SmartPtr<TFM> exTFM = exTFMFont->getTFM();
	  x_height = exTFM->getDimension(index) * accentFont->getScale();
	}

      const SmartPtr<TFM> baseTFM = baseFont->getTFM();
      const ComputerModernFamily::FontNameId baseNameId = fontNameIdOfTFM(baseTFM);
      bool math = false;

      switch (baseNameId) {
	// these are all the TeX text fonts 
      case ComputerModernFamily::FN_CMR:
      case ComputerModernFamily::FN_CMB:
      case ComputerModernFamily::FN_CMBX:
      case ComputerModernFamily::FN_CMBXTI:
      case ComputerModernFamily::FN_CMTI:
      case ComputerModernFamily::FN_CMSS:
      case ComputerModernFamily::FN_CMSSI:
      case ComputerModernFamily::FN_CMSSBX:
      case ComputerModernFamily::FN_CMTT:
	math = false;
	break;
	// these are all the TeX math fonts 
      case ComputerModernFamily::FN_CMSY:
      case ComputerModernFamily::FN_CMBSY:
      case ComputerModernFamily::FN_CMMI:
      case ComputerModernFamily::FN_CMMIB: 
      case ComputerModernFamily::FN_CMEX:
      case ComputerModernFamily::FN_MSAM:
      case ComputerModernFamily::FN_MSBM:
	math = true;
	break;
      default:
	assert(false); /* IMPOSSIBLE */
      }

      // printf("modo %d accentData? %d\n", math, accentData);

      if (math)
	{
	  if (accentData)
	    dy = base->box().height - (std::min(base->box().height, x_height));
	  else
	    {
	      /* TODO */
	    }

	  const ComputerModernFamily::FontEncId fontEncId = ComputerModernFamily::encIdOfFontNameId(baseNameId);
	  if ((fontEncId == ComputerModernFamily::FE_CMMI ||       //family 1 (math italic font)
	       fontEncId == ComputerModernFamily::FE_CMSY) &&      //family 2 (math symbol font)
	      !(accentNameId == ComputerModernFamily::FN_CMBXTI ||
		accentNameId == ComputerModernFamily::FN_CMTI ||
		accentNameId == ComputerModernFamily::FN_CMMI ||
		accentNameId == ComputerModernFamily::FN_CMSSI))
	    {
	      // printf("Imposto lo skewchar\n");
	      const Char8 skewChar = (fontEncId == ComputerModernFamily::FE_CMMI) ? 0177 : 0060;
	      scaled kerning;

	      if (baseTFM->getGlyphKerning(baseIndex, skewChar, kerning))
		{
		  kerning *= baseFont->getScale();
		  dx += kerning;
		}
	    }
	}
      else
	{
	  if (accentData)
	    {
	      const unsigned int slantIndex = 1;  // position of slant
	      const scaled baseSlant = baseFont->getTFM()->getDimension(slantIndex);
	      const scaled accentSlant = accentTFM->getDimension(slantIndex);
      
	      dx = dx + base->box().height * baseSlant - x_height * accentSlant;
	      dy = base->box().height - x_height;
	    }
	  else
	    {
	      /* TODO */
	    }
	}
      res = true;
    }

  else
    res = Shaper::computeCombiningCharOffsetsAbove(base, script, dx, dy);

  return res;
}


#if 0
/*  
  SmartPtr<TFMFont> exTFMFont = getFont(ComputerModernFamily::FN_CMR,
					ComputerModernFamily::FS_10, 
				        baseFont->getScale());

  unsigned index = 5;  //position of x-height in CMR Font
  SmartPtr<TFM> exTFM = exTFMFont->getTFM();
  dy = -(std::min(base->box().height, (exTFM->getDimension(index) * baseFont->getScale())));
*/

  if (getGlyphData(accent, accentFont, accentIndex))
    {
      
      if (accentNameId == ComputerModernFamily::FN_CMBXTI ||
	  accentNameId == ComputerModernFamily::FN_CMTI ||
	  accentNameId == ComputerModernFamily::FN_CMMI ||
	  accentNameId == ComputerModernFamily::FN_CMSSI)
	{
          if (base->box().height != (exTFM->getDimension(index) * baseFont->getScale()))
          { 
//	    dx = scaled::zero();
//            return true;
//	    unsigned int index1 = 1;
//	    scaled baseSlant = baseFont->getTFM()->getDimension(index1);
//	    scaled accentSlant = accentTFM->getDimension(index1);
//	    dx = dx + base->box().height * baseSlant - accent->box().height * accentSlant;
	    dx += baseFont->getTFM()->getGlyphItalicCorrection(baseIndex) * baseFont->getScale();
          }
	    return true;
	}
    } 
  return true;
}
#endif 
