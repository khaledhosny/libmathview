// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#include <cassert>

#include "AreaFactory.hh"
#include "MathMLElement.hh"
#include "MathGraphicDevice.hh"
#include "MathVariantMap.hh"
#include "FormattingContext.hh"

MathGraphicDevice::MathGraphicDevice()
  : shaperManager(ShaperManager::create())
{ }

MathGraphicDevice::~MathGraphicDevice()
{
  shaperManager->unregisterShapers();
}

scaled
MathGraphicDevice::ex(const FormattingContext& context) const
{
  return string(context, "x")->box().height;
}

scaled
MathGraphicDevice::axis(const FormattingContext& context) const
{
  const BoundingBox pbox = string(context, "+")->box();
  // the + is a better choice rather than x because its vertical extent
  // is certainly an odd number of pixels, whereas the x has almost
  // certainly an even number of pixels. This way it is reduced the
  // approximation due to the / 2  
  return (pbox.height - pbox.depth) / 2;
}

AreaRef
MathGraphicDevice::wrapper(const FormattingContext&, const AreaRef& area) const
{
  return factory->box(area, area->box());
}

AreaRef
MathGraphicDevice::dummy(const FormattingContext& context) const
{
  return getFactory()->color(string(context, StringOfUCS4String(UCS4String(1, 0xfffd))), RGBColor::RED());
}

#include "CachedShapedString.hh"
#include "HashMap.hh"
typedef HASH_MAP_NS::hash_map<CachedShapedStringKey, AreaRef, CachedShapedStringKeyHash> ShapedStringCache;
typedef HASH_MAP_NS::hash_map<CachedShapedStringKey, AreaRef, CachedShapedStringKeyHash> ShapedStretchyStringCache;

AreaRef
MathGraphicDevice::string(const FormattingContext& context,
			  const String& str) const
{
  if (str.length() == 0)
    return dummy(context);
  else if (context.getMathMLElement() == context.getStretchOperator())
    {
      static ShapedStretchyStringCache cache;
      CachedShapedStretchyStringKey key(str, context.getSize(), context.getStretchH(), context.getStretchV());
#if 1
      std::pair<ShapedStringCache::iterator, bool> r = cache.insert(std::make_pair(key, AreaRef(0)));
      if (r.second)
	{
	  UCS4String source = UCS4StringOfString(str);
	  mapMathVariant(context.getVariant(), source);
	  r.first->second = getShaperManager()->shapeStretchy(context, source,
							      context.getStretchV(),
							      context.getStretchH());
	  return r.first->second;
	}
      else
	return r.first->second;
#else
      ShapedStretchyStringCache::const_iterator p = cache.find(key);
      if (p != cache.end())
	return p->second;
      else
	{
	  UCS4String source = UCS4StringOfString(str);
	  mapMathVariant(context.getVariant(), source);
	  return (cache[key] = getShaperManager()->shapeStretchy(context, source,
								 context.getStretchV(),
								 context.getStretchH()));
	}
#endif
    }
  else
    {
      static ShapedStringCache cache;
      CachedShapedStringKey key(str, context.getSize());

#if 1
      std::pair<ShapedStringCache::iterator, bool> r = cache.insert(std::make_pair(key, AreaRef(0)));
      if (r.second)
	{
	  UCS4String source = UCS4StringOfString(str);
	  mapMathVariant(context.getVariant(), source);
	  r.first->second = getShaperManager()->shape(context, source);
	  return r.first->second;
	}
      else
	return r.first->second;
#else
      ShapedStringCache::const_iterator p = cache.find(key);
      if (p != cache.end())
	return p->second;
      else
	{
	  UCS4String source = UCS4StringOfString(str);
	  mapMathVariant(context.getVariant(), source);
	  return (cache[key] = getShaperManager()->shape(context, source));
	}
#endif
    }
}

AreaRef
MathGraphicDevice::stretchStringV(const FormattingContext& context,
				  const String& str,
				  const scaled& height,
				  const scaled& depth) const
{
  return getShaperManager()->shapeStretchy(context, UCS4StringOfString(str), height, depth);
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

  AreaRef s = factory->verticalSpace(context.getDisplayStyle() ? RULE * 3 : RULE, scaled::zero());

  v.reserve(5);
  v.push_back(denominator);
  v.push_back(s);
  v.push_back(factory->horizontalLine(evaluate(context, lineThickness, RULE), context.getColor()));
  v.push_back(s);
  v.push_back(numerator);

  return factory->shift(factory->verticalArray(v, 2), axis(context));
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
  
  return factory->horizontalArray(h);
}

AreaRef
MathGraphicDevice::radical(const FormattingContext& context,
			   const AreaRef& base,
			   const AreaRef& index) const
{
  const scaled RULE = defaultLineThickness(context);
  UCS4String root(1, 0x221a);
  BoundingBox baseBox = base->box();
  AreaRef rootArea = stretchStringV(context, StringOfUCS4String(root), baseBox.depth, baseBox.height + RULE);
  
  std::vector<AreaRef> v;
  v.reserve(3);
  v.push_back(base);
  v.push_back(factory->verticalSpace(RULE, 0));
  v.push_back(factory->horizontalLine(RULE, context.getColor()));

  AreaRef baseArea = factory->verticalArray(v, 0);

  std::vector<AreaRef> h;
  h.reserve(index ? 3 : 2);
  if (index) h.push_back(index);
  h.push_back(factory->shift(rootArea, baseArea->box().height - rootArea->box().height));
  h.push_back(baseArea);

  return factory->horizontalArray(h);
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

  scaled EX = ex(context);
  scaled AXIS = axis(context);
  scaled RULE = defaultLineThickness(context);

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
  if (subScript) o.push_back(factory->shift(subScript, -subScriptShift));
  if (superScript) o.push_back(factory->shift(superScript, superScriptShift));

  std::vector<AreaRef> h;
  h.reserve(2);
  h.push_back(base);
  if (o.size() > 1)
    h.push_back(factory->overlapArray(o));
  else
    h.push_back(o.front());

  return factory->horizontalArray(h);
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
      if (*preP) o.push_back(factory->right(factory->shift(*preP, -subScriptShift)));
      if (*preQ) o.push_back(factory->right(factory->shift(*preQ, superScriptShift)));

      if (o.size() > 1)
	h.push_back(factory->overlapArray(o));
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
      if (*postP) o.push_back(factory->shift(*postP, -subScriptShift));
      if (*postQ) o.push_back(factory->shift(*postQ, superScriptShift));

      if (o.size() > 1)
	h.push_back(factory->overlapArray(o));
      else
	h.push_back(o.front());
    }

  return factory->horizontalArray(h);
}

AreaRef
MathGraphicDevice::underOver(const FormattingContext& context,
			     const AreaRef& base,
			     const AreaRef& underScript, bool accentUnder,
			     const AreaRef& overScript, bool accent) const
{
  std::vector<AreaRef> v;
  v.reserve(3);
  if (underScript) v.push_back(factory->center(underScript));
  v.push_back(factory->center(base));
  if (overScript) v.push_back(factory->center(overScript));

  return factory->verticalArray(v, underScript ? 1 : 0);
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

      AreaRef vobj = factory->verticalLine(defaultLineThickness(context), context.getColor());
      AreaRef hobj = factory->horizontalLine(defaultLineThickness(context), context.getColor());

      if (notation == "box" || notation == "longdiv" || notation == "left") c.push_back(vobj);
      c.push_back(res);
      if (notation == "box" || notation == "actuarial" || notation == "right") c.push_back(vobj);
      if (c.size() > 1) res = factory->horizontalArray(c);

      c.clear();
      if (notation == "box" || notation == "bottom") c.push_back(hobj);
      c.push_back(res);
      if (notation == "box" || notation == "longdiv" || notation == "actuarial" || notation == "top") c.push_back(hobj);
      if (c.size() > 1) res = factory->verticalArray(c, (notation == "top") ? 0 : 1);

      c.clear();
      c.push_back(res);
      if (notation == "verticalstrike") c.push_back(factory->center(vobj));
      else if (notation == "horizontalstrike") c.push_back(factory->middle(hobj));
      else if (notation == "baselinestrike") c.push_back(hobj);
      if (c.size() > 1) res = factory->overlapArray(c);

      return res;
    }
}
