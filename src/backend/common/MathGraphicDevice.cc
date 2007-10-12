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

#include <cassert>

#include "AreaFactory.hh"
#include "MathMLElement.hh"
#include "MathGraphicDevice.hh"
#include "MathVariantMap.hh"
#include "FormattingContext.hh"
#include "ShaperManager.hh"
#include "String.hh"
#include "Area.hh"
#include "GlyphArea.hh"

MathGraphicDevice::MathGraphicDevice(const SmartPtr<AbstractLogger>& logger)
  : GraphicDevice(logger)
{ }

MathGraphicDevice::~MathGraphicDevice()
{ }

scaled
MathGraphicDevice::ex(const FormattingContext& context) const
{
  return unstretchedString(context, "x")->box().height;
}

scaled
MathGraphicDevice::axis(const FormattingContext& context) const
{
  const BoundingBox pbox = unstretchedString(context, "+")->box();
  // the + is a better choice rather than x because its vertical extent
  // is certainly an odd number of pixels, whereas the x has almost
  // certainly an even number of pixels. This way it is reduced the
  // approximation due to the / 2  
  return (pbox.height - pbox.depth) / 2;
}

AreaRef
MathGraphicDevice::wrapper(const FormattingContext&, const AreaRef& area) const
{
  return getFactory()->box(area, area->box());
}

AreaRef
MathGraphicDevice::dummy(const FormattingContext& context) const
{
  return getFactory()->color(unstretchedString(context, StringOfUCS4String(UCS4String(1, 0xfffd))), RGBColor::RED());
}

#include "CachedShapedString.hh"
#include "HashMap.hh"
typedef HASH_MAP_NS::hash_map<CachedShapedStringKey, AreaRef, CachedShapedStringKeyHash> ShapedStringCache;
typedef HASH_MAP_NS::hash_map<CachedShapedStretchyStringKey, AreaRef, CachedShapedStringKeyHash> ShapedStretchyStringCache;

static ShapedStretchyStringCache stretchyStringCache;
static ShapedStringCache stringCache;

void
MathGraphicDevice::clearCache() const
{
  stretchyStringCache.clear();
  stringCache.clear();
}

AreaRef
MathGraphicDevice::stretchedString(const FormattingContext& context, const String& str) const
{
  CachedShapedStretchyStringKey key(str, context.getVariant(), context.getSize(),
				    context.getStretchH(), context.getStretchV());
#if 1
  std::pair<ShapedStretchyStringCache::iterator, bool> r = stretchyStringCache.insert(std::make_pair(key, AreaRef(0)));
  if (r.second)
    {
      UCS4String source = UCS4StringOfString(str);
      if (context.getMathMode())
	mapMathVariant(context.getVariant(), source);
      r.first->second = getShaperManager()->shapeStretchy(context,
							  context.getMathMLElement(),
							  context.MGD()->getFactory(),
							  source,
							  context.getStretchV(),
							  context.getStretchH());
      return r.first->second;
    }
  else
    return r.first->second;
#else
  ShapedStretchyStringCache::const_iterator p = stretchyStringCache.find(key);
  if (p != stretchyStringCache.end())
    return p->second;
  else
    {
      UCS4String source = UCS4StringOfString(str);
      mapMathVariant(context.getVariant(), source);
      return (stretchyStringCache[key] = getShaperManager()->shapeStretchy(context,
									   context.getMathMLElement(),
									   context.MGD()->getFactory(),
									   source,
									   context.getStretchV(),
									   context.getStretchH()));
    }
#endif
}

AreaRef
MathGraphicDevice::unstretchedString(const FormattingContext& context, const String& str) const
{
  CachedShapedStringKey key(str, context.getVariant(), context.getSize());

#if 1
  std::pair<ShapedStringCache::iterator, bool> r = stringCache.insert(std::make_pair(key, AreaRef(0)));
  if (r.second)
    {
      UCS4String source = UCS4StringOfString(str);
      if (context.getMathMode())
	mapMathVariant(context.getVariant(), source);
      r.first->second = getShaperManager()->shape(context,
						  context.getMathMLElement(),
						  context.MGD()->getFactory(),
						  source);
      return r.first->second;
    }
  else
    return r.first->second;
#else
  ShapedStringCache::const_iterator p = stringCache.find(key);
  if (p != stringCache.end())
    return p->second;
  else
    {
      UCS4String source = UCS4StringOfString(str);
      mapMathVariant(context.getVariant(), source);
      return (stringCache[key] = getShaperManager()->shape(context,
							   context.getMathMLElement(),
							   context.MGD()->getFactory(),
							   source));
    }
#endif
}

AreaRef
MathGraphicDevice::string(const FormattingContext& context,
			  const String& str) const
{
  if (str.length() == 0)
    return dummy(context);
  else if (context.getMathMLElement() == context.getStretchOperator())
    return stretchedString(context, str);
  else
    return unstretchedString(context, str);
}

AreaRef
MathGraphicDevice::stretchStringV(const FormattingContext& context,
				  const String& str,
				  const scaled& height,
				  const scaled& depth) const
{
  return getShaperManager()->shapeStretchy(context,
					   context.getMathMLElement(),
					   context.MGD()->getFactory(),
					   UCS4StringOfString(str), height + depth, 0);
}

AreaRef
MathGraphicDevice::glyph(const FormattingContext& context,
			 const String& alt,
			 const String& fontFamily,
			 unsigned long index) const
{
  assert(false);
}

AreaRef
MathGraphicDevice::fraction(const FormattingContext& context,
			    const AreaRef& numerator,
			    const AreaRef& denominator,
			    const Length& lineThickness) const
{
  const scaled RULE = defaultLineThickness(context);

  std::vector<AreaRef> v;

  AreaRef s = getFactory()->verticalSpace(context.getDisplayStyle() ? RULE * 3 : RULE, scaled::zero());

  v.reserve(5);
  v.push_back(denominator);
  v.push_back(s);
  v.push_back(getFactory()->horizontalLine(evaluate(context, lineThickness, RULE), context.getColor()));
  v.push_back(s);
  v.push_back(numerator);

  return getFactory()->shift(getFactory()->verticalArray(v, 2), axis(context));
}

AreaRef
MathGraphicDevice::bevelledFraction(const FormattingContext& context,
				    const AreaRef& numerator,
				    const AreaRef& denominator,
				    const Length& lineThickness) const
{
  BoundingBox n = numerator->box();
  BoundingBox d = denominator->box();

  std::vector<AreaRef> h;
  h.reserve(3);
  h.push_back(numerator);
  h.push_back(stretchStringV(context, "/", std::max(n.height, d.height), std::max(n.depth, d.depth)));
  h.push_back(denominator);
  
  return getFactory()->horizontalArray(h);
}

AreaRef
MathGraphicDevice::radical(const FormattingContext& context,
			   const AreaRef& base,
			   const AreaRef& index) const
{
  const scaled RULE = defaultLineThickness(context);
  const UCS4String root(1, 0x221a);
  const BoundingBox baseBox = base->box();
  const AreaRef rootArea = stretchStringV(context, StringOfUCS4String(root), baseBox.height + 2 * RULE, baseBox.depth);
  const BoundingBox rootBox = rootArea->box();

  std::vector<AreaRef> v;
  v.reserve(3);
  v.push_back(base);
  v.push_back(getFactory()->verticalSpace(RULE, 0));
  v.push_back(getFactory()->horizontalLine(RULE, context.getColor()));

  const AreaRef baseArea = getFactory()->verticalArray(v, 0);

  std::vector<AreaRef> h;
  h.reserve(index ? 4 : 2);
  if (index)
    {
      const Length zero(0.0f, Length::PT_UNIT);
      const BoundingBox indexBox = index->box();
      scaled u;
      scaled v;
      calculateScriptShift(context, baseArea->box(), BoundingBox(), zero, indexBox, zero, v, u);
      h.push_back(getFactory()->shift(index, u));
      h.push_back(getFactory()->horizontalSpace(-rootBox.width / 2));
    }
  h.push_back(getFactory()->shift(rootArea, baseArea->box().height - rootBox.height));
  h.push_back(baseArea);

  return getFactory()->horizontalArray(h);
}

void
MathGraphicDevice::calculateDefaultScriptShift(const FormattingContext& context,
					       const BoundingBox& baseBox,
					       const BoundingBox& subScriptBox,
					       const BoundingBox& superScriptBox,
					       scaled& v,
					       scaled& u) const
{
  assert(baseBox.defined());

  const scaled EX = ex(context);
  const scaled AXIS = axis(context);
  const scaled RULE = defaultLineThickness(context);

  u = std::max(EX, baseBox.height - AXIS);
  v = std::max(AXIS, baseBox.depth + AXIS);

  if (!superScriptBox.defined())
    {
      u = 0;
      v = std::max(v, subScriptBox.height - (EX * 4) / 5);
    }
  else
    {
      u = std::max(u, superScriptBox.depth + EX / 4);
      if (!subScriptBox.defined())
	{
	  v = 0;
	}
      else
	{
          if ((u - superScriptBox.depth) - (subScriptBox.height - v) < 4 * RULE)
            {
              v = 4 * RULE - u + superScriptBox.depth + subScriptBox.height;
  
              scaled psi = (4 * EX) / 5 - (u - superScriptBox.depth);
              if (psi > scaled::zero())
                {
                  u += psi;
                  v -= psi;
                }
            }
        }
    }
}

void
MathGraphicDevice::calculateScriptShift(const FormattingContext& context,
					const BoundingBox& baseBox,
					const BoundingBox& subScriptBox,
					const Length& subScriptMinShift,
					const BoundingBox& superScriptBox,
					const Length& superScriptMinShift,
					scaled& v,
					scaled& u) const
{
  calculateDefaultScriptShift(context, baseBox, subScriptBox, superScriptBox, v, u);
  v = std::max(v, evaluate(context, subScriptMinShift, v));
  u = std::max(u, evaluate(context, superScriptMinShift, u));
}

AreaRef
MathGraphicDevice::script(const FormattingContext& context,
			  const AreaRef& base,
			  const AreaRef& subScript,
			  const Length& subScriptMinShift,
			  const AreaRef& superScript,
			  const Length& superScriptMinShift) const
{
  assert(base);

  scaled subScriptShift;
  scaled superScriptShift;

  calculateScriptShift(context,
		       base->box(),
		       subScript ? subScript->box() : BoundingBox(),
		       subScriptMinShift,
		       superScript ? superScript->box() : BoundingBox(),
		       superScriptMinShift,
		       subScriptShift,
		       superScriptShift);

  std::vector<AreaRef> o;
  o.reserve(2);
  if (subScript) o.push_back(getFactory()->shift(subScript, -subScriptShift));
  if (superScript) o.push_back(getFactory()->shift(superScript, superScriptShift));

  std::vector<AreaRef> h;
  h.reserve(2);
  h.push_back(base);
  if (o.size() > 1)
    h.push_back(getFactory()->overlapArray(o));
  else
    h.push_back(o.front());

  return getFactory()->horizontalArray(h);
}

AreaRef
MathGraphicDevice::multiScripts(const FormattingContext& context,
				const AreaRef& base,
				const std::vector<AreaRef>& subScripts,
				const std::vector<AreaRef>& preSubScripts,
				const Length& subScriptMinShift,
				const std::vector<AreaRef>& superScripts,
				const std::vector<AreaRef>& preSuperScripts,
				const Length& superScriptMinShift) const
{
  assert(subScripts.size() == superScripts.size());
  assert(preSubScripts.size() == preSuperScripts.size());

  BoundingBox subScriptsBox;
  BoundingBox superScriptsBox;

  for (std::vector<AreaRef>::const_iterator p = subScripts.begin();
       p != subScripts.end();
       p++)
    if (*p) subScriptsBox.append((*p)->box());
  for (std::vector<AreaRef>::const_iterator p = preSubScripts.begin();
       p != preSubScripts.end();
       p++)
    if (*p) subScriptsBox.append((*p)->box());

  for (std::vector<AreaRef>::const_iterator p = superScripts.begin();
       p != superScripts.end();
       p++)
    if (*p) superScriptsBox.append((*p)->box());
  for (std::vector<AreaRef>::const_iterator p = preSuperScripts.begin();
       p != preSuperScripts.end();
       p++)
    if (*p) superScriptsBox.append((*p)->box());

  scaled subScriptShift;
  scaled superScriptShift;

  calculateScriptShift(context,
		       base->box(),
		       subScriptsBox, subScriptMinShift,
		       superScriptsBox, superScriptMinShift,
		       subScriptShift,
		       superScriptShift);

  std::vector<AreaRef> h;
  h.reserve(subScripts.size() + preSubScripts.size() + 1);

  std::vector<AreaRef>::const_reverse_iterator preP = preSubScripts.rbegin();
  std::vector<AreaRef>::const_reverse_iterator preQ = preSuperScripts.rbegin();
  for (; preP != preSubScripts.rend(); preP++, preQ++)
    {
      std::vector<AreaRef> o;
      o.reserve(2);
      if (*preP) o.push_back(getFactory()->right(getFactory()->shift(*preP, -subScriptShift)));
      if (*preQ) o.push_back(getFactory()->right(getFactory()->shift(*preQ, superScriptShift)));

      if (o.size() > 1)
	h.push_back(getFactory()->overlapArray(o));
      else
	h.push_back(o.front());
    }

  h.push_back(base);

  std::vector<AreaRef>::const_iterator postP = subScripts.begin();
  std::vector<AreaRef>::const_iterator postQ = superScripts.begin();
  for (; postP != subScripts.end(); postP++, postQ++)
    {
      std::vector<AreaRef> o;
      o.reserve(2);
      if (*postP) o.push_back(getFactory()->shift(*postP, -subScriptShift));
      if (*postQ) o.push_back(getFactory()->shift(*postQ, superScriptShift));

      if (o.size() > 1)
	h.push_back(getFactory()->overlapArray(o));
      else
	h.push_back(o.front());
    }

  return getFactory()->horizontalArray(h);
}

#if 0
#include <iostream>
#include <stdio.h>
#endif

AreaRef
MathGraphicDevice::underOver(const FormattingContext& context,
			     const AreaRef& base,
			     const AreaRef& underScript, bool accentUnder,
			     const AreaRef& overScript, bool accent) const
{
  SmartPtr<const GlyphStringArea> baseStringArea = base ? base->getGlyphStringArea() : NULL;
  SmartPtr<const GlyphStringArea> overStringArea = overScript ? overScript->getGlyphStringArea()
						 	        : NULL;
  SmartPtr<const GlyphStringArea> underStringArea = underScript ? 
						    underScript->getGlyphStringArea() : NULL;

#if 0
  printf("base GlyphArea %s\n", base->getGlyphArea() ? "true" : "false");
  printf("over GlyphArea %s\n", overScript->getGlyphArea() ? "true" : "false");  

  printf("base GlyphStringArea %s\n", base->getGlyphStringArea() ? "true" : "false"); 
  printf("over GlyphStringArea %s\n", base->getGlyphStringArea() ? "true" : "false"); 
#endif

  if (baseStringArea)
  {
    UCS4String baseSource;
    UCS4String overSource;
    UCS4String underSource;
    AreaRef res;
    
    //controls if overScript and/or underScript are very sinlge character
    bool overCondition = overStringArea && accent && 
			 ((overSource = overStringArea->getSource()).length() == 1);
    bool underCondition = underStringArea && accentUnder &&
	   		  ((underSource = underStringArea->getSource()).length() == 1);

    baseSource = baseStringArea->getSource();
    //controls if the base character is a single char
    if (baseSource.length() == 1)
    {
      if (overScript && underScript)
      {
        if (overCondition)
        {
	  //we have that overScript is a single character
	  res = shaperManager->compose(context,
				       base, baseSource,
				       overScript, overSource, true);

	  //if this condition is true then also underScript is a single char
	  if (underCondition)
            res = shaperManager->compose(context,
					 res, baseSource,
					 underScript, underSource, false);

          //we have that the overScript is a single char
          //but the underScript isn't a single char
          else
	    res = underOver(context, res, underScript, accentUnder, NULL, accent);  

	  return res;
        }
        //we have that only the underScript is a single char  
        else if (underCondition)
	{
	  res = shaperManager->compose(context,
				       base, baseSource,
				       underScript, underSource, false);
	  res = underOver(context, res, NULL, accentUnder, overScript, accent);   

	  return res;
        }
      }

      //we control if one of overScript and underScript
      //is a single char
      else if (overScript || underScript)
      {
	if (overCondition)
	{
          res = shaperManager->compose(context,
		     	  	       base, baseSource,
			               overScript, overSource, true);
          return res;
        }
        if (underCondition)
	{
	  res = shaperManager->compose(context,
		     	  	       base, baseSource,
			               underScript, underSource, false);
          return res;
        }
      }
    }
  }
  
  //the next instructions represent the default behavior
  //in which overScript and underScript aren't single char
  const scaled RULE = defaultLineThickness(context);
  const AreaRef singleSpace = getFactory()->verticalSpace(RULE, 0);
  const AreaRef tripleSpace = getFactory()->verticalSpace(3 * RULE, 0);

  int n = 1;
  if (underScript && !accentUnder) n += 3;
  if (overScript && !accent) n += 3;

  std::vector<AreaRef> v;
  v.reserve(n);
  if (underScript)
  {
    if (!accentUnder) v.push_back(singleSpace);
    v.push_back(getFactory()->center(underScript));
    if (!accentUnder) v.push_back(tripleSpace);
  }
  v.push_back(getFactory()->center(base));
  if (overScript)
  {
    if (!accent) v.push_back(tripleSpace);
    v.push_back(getFactory()->center(overScript));
    if (!accent) v.push_back(singleSpace);
  }

  return getFactory()->verticalArray(v, underScript ? (accentUnder ? 1 : 3) : 0);
}

AreaRef
MathGraphicDevice::enclose(const FormattingContext& context,
			   const AreaRef& base,
			   const String& notation) const
{
  if (notation == "radical") 
    return radical(context, base, 0);
  else
    {
      std::vector<AreaRef> c;
      c.reserve(3);

      AreaRef res = base;

      AreaRef vobj = getFactory()->verticalLine(defaultLineThickness(context), context.getColor());
      AreaRef hobj = getFactory()->horizontalLine(defaultLineThickness(context), context.getColor());

      if (notation == "box" || notation == "longdiv" || notation == "left") c.push_back(vobj);
      c.push_back(res);
      if (notation == "box" || notation == "actuarial" || notation == "right") c.push_back(vobj);
      if (c.size() > 1) res = getFactory()->horizontalArray(c);

      c.clear();
      if (notation == "box" || notation == "bottom") c.push_back(hobj);
      c.push_back(res);
      if (notation == "box" || notation == "longdiv" || notation == "actuarial" || notation == "top") c.push_back(hobj);
      if (c.size() > 1) res = getFactory()->verticalArray(c, (notation == "top") ? 0 : 1);

      c.clear();
      c.push_back(res);
      if (notation == "verticalstrike") c.push_back(getFactory()->center(vobj));
      else if (notation == "horizontalstrike") c.push_back(getFactory()->middle(hobj));
      else if (notation == "baselinestrike") c.push_back(hobj);
      if (c.size() > 1) res = getFactory()->overlapArray(c);

      return res;
    }
}
