// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// Copyright (C) 2013, Khaled Hosny <khaledhosny@eglug.org>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#include <config.h>

#include <cassert>

#include "AreaFactory.hh"
#include "MathGraphicDevice.hh"
#include "MathMLElement.hh"
#include "MathVariantMap.hh"
#include "FormattingContext.hh"
#include "ShaperManager.hh"
#include "String.hh"
#include "Area.hh"
#include "GlyphArea.hh"

MathGraphicDevice::MathGraphicDevice(const SmartPtr<MathFont>& font)
  : mathFont(font)
{ }

MathGraphicDevice::~MathGraphicDevice()
{ }

scaled
MathGraphicDevice::getConstant(const FormattingContext& context,
                               MathConstant constant) const
{
  if (context.getDisplayStyle())
    {
      // switch to display style version of the requested constant
      switch (constant)
        {
          case stackTopShiftUp:
            constant = stackTopDisplayStyleShiftUp;
            break;
          case stackBottomShiftDown:
            constant = stackBottomDisplayStyleShiftDown;
            break;
          case stackGapMin:
            constant = stackDisplayStyleGapMin;
            break;
          case fractionNumeratorShiftUp:
            constant = fractionNumeratorDisplayStyleShiftUp;
            break;
          case fractionDenominatorShiftDown:
            constant = fractionDenominatorDisplayStyleShiftDown;
            break;
          case fractionNumeratorGapMin:
            constant = fractionNumeratorDisplayStyleGapMin;
            break;
          case fractionDenominatorGapMin:
            constant = fractionDenominatorDisplayStyleGapMin;
            break;
          case radicalVerticalGap:
            constant = radicalDisplayStyleVerticalGap;
            break;
          default:
            break;
        }
    }

  scaled value = mathFont->getConstant(constant);
  // scale the non-percent constants
  switch (constant)
    {
      case scriptPercentScaleDown:
      case scriptScriptPercentScaleDown:
      case radicalDegreeBottomRaisePercent:
        break;
      default:
        value = (value * context.getSize()) / mathFont->getUnitsPerEM();
        break;
    }

  return value;
}

scaled
MathGraphicDevice::getRuleThickness(const FormattingContext& context,
                                    MathConstant constant) const
{
  // at least 1px thick
  return std::max(getConstant(context, constant), scaled(72.27f / dpi(context)));
}

scaled
MathGraphicDevice::ex(const FormattingContext& context) const
{
  // XXX
  return getConstant(context, accentBaseHeight);
}

scaled
MathGraphicDevice::axis(const FormattingContext& context) const
{
  return getConstant(context, axisHeight);
}

scaled
MathGraphicDevice::defaultLineThickness(const FormattingContext& context) const
{
  // XXX
  return getRuleThickness(context, fractionRuleThickness);
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
#include <unordered_map>
typedef std::unordered_map<CachedShapedStringKey, AreaRef, CachedShapedStringKeyHash> ShapedStringCache;
typedef std::unordered_map<CachedShapedStretchyStringKey, AreaRef, CachedShapedStringKeyHash> ShapedStretchyStringCache;

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
  std::pair<ShapedStretchyStringCache::iterator, bool> r = stretchyStringCache.insert(std::make_pair(key, AreaRef(0)));
  if (r.second)
    {
      UCS4String source = UCS4StringOfString(str);
      if (context.getMathMode())
        mapMathVariant(context.getVariant(), source);
      r.first->second = getShaperManager()->shapeStretchy(context,
                                                          source,
                                                          context.getStretchV(),
                                                          context.getStretchH());
      return r.first->second;
    }
  else
    return r.first->second;
}

AreaRef
MathGraphicDevice::unstretchedString(const FormattingContext& context, const String& str) const
{
  CachedShapedStringKey key(str, context.getVariant(), context.getSize());

  std::pair<ShapedStringCache::iterator, bool> r = stringCache.insert(std::make_pair(key, AreaRef(0)));
  if (r.second)
    {
      UCS4String source = UCS4StringOfString(str);
      if (context.getMathMode())
        mapMathVariant(context.getVariant(), source);
      r.first->second = getShaperManager()->shape(context, source);
      return r.first->second;
    }
  else
    return r.first->second;
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
  return getShaperManager()->shapeStretchy(context, UCS4StringOfString(str), height + depth, 0);
}

AreaRef
MathGraphicDevice::glyph(const FormattingContext& /*context*/,
                         const String& /*alt*/,
                         const String& /*fontFamily*/,
                         unsigned long /*index*/) const
{
  assert(false);
}

AreaRef
MathGraphicDevice::fraction(const FormattingContext& context,
                            const AreaRef& numerator,
                            const AreaRef& denominator,
                            const Length& lineThickness) const
{
  const scaled RULE = getRuleThickness(context, fractionRuleThickness);
  const scaled AXIS = axis(context);
  const scaled ruleThickness = evaluate(context, lineThickness, RULE);

  BoundingBox n = numerator->box();
  BoundingBox d = denominator->box();

  std::vector<AreaRef> v;

  if (ruleThickness == scaled::zero())
    {
      scaled shift_up = getConstant(context, stackTopShiftUp);
      scaled shift_down = getConstant(context, stackBottomShiftDown);
      scaled clr = getConstant(context, stackGapMin);
      scaled delta = (clr - ((shift_up - n.depth) - (d.height - shift_down))) / 2;
      if (delta > scaled::zero())
        {
          shift_up += delta;
          shift_down += delta;
        }

      v.reserve(3);
      v.push_back(denominator);
      v.push_back(getFactory()->verticalSpace((shift_up - n.depth) - (d.height - shift_down), 0));
      v.push_back(numerator);
    }
  else
    {
      scaled shift_up = getConstant(context, fractionNumeratorShiftUp);
      scaled shift_down = getConstant(context, fractionDenominatorShiftDown);
      scaled delta = ruleThickness / 2;

      scaled clr1 = getConstant(context, fractionNumeratorGapMin);
      scaled delta1 = clr1 - ((shift_up - n.depth) - (AXIS + delta));
      if (delta1 > scaled::zero())
        shift_up = shift_up + delta1;

      scaled clr2 = getConstant(context, fractionDenominatorGapMin);
      scaled delta2 = clr2 - ((AXIS - delta) - (d.height - shift_down));
      if (delta2 > scaled::zero())
        shift_down = shift_down + delta2;

      v.reserve(5);
      v.push_back(denominator);
      v.push_back(getFactory()->verticalSpace((AXIS - delta) - (d.height - shift_down), 0));
      v.push_back(getFactory()->horizontalLine(ruleThickness, context.getColor()));
      v.push_back(getFactory()->verticalSpace((shift_up - n.depth) - (AXIS + delta), 0));
      v.push_back(numerator);
    }

  return getFactory()->shift(getFactory()->verticalArray(v, 2), AXIS);
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
  const scaled RULE = getRuleThickness(context, radicalRuleThickness);
  const scaled GAP = getConstant(context, radicalVerticalGap);
  const scaled KERN = getConstant(context, radicalExtraAscender);
  const UCS4String root(1, 0x221a);
  const BoundingBox baseBox = base->box();
  const AreaRef rootArea = stretchStringV(context, StringOfUCS4String(root), baseBox.height + GAP + RULE, baseBox.depth);
  const BoundingBox rootBox = rootArea->box();

  scaled clr = GAP;
  scaled delta = (rootBox.verticalExtent() - RULE) - (baseBox.verticalExtent() + GAP);
  if (delta > scaled::zero())
    clr += delta / 2;

  std::vector<AreaRef> v;
  v.reserve(4);
  v.push_back(base);
  v.push_back(getFactory()->verticalSpace(clr, 0));
  v.push_back(getFactory()->horizontalLine(RULE, context.getColor()));
  v.push_back(getFactory()->verticalSpace(KERN, 0));

  const AreaRef baseArea = getFactory()->verticalArray(v, 0);

  std::vector<AreaRef> h;
  h.reserve(index ? 5 : 2);
  if (index)
    {
      const scaled KERN_BEFORE = getConstant(context, radicalKernBeforeDegree);
      const scaled KERN_AFTER = getConstant(context, radicalKernAfterDegree);
      const scaled RAISE = getConstant(context, radicalDegreeBottomRaisePercent);

      scaled shift = baseArea->box().verticalExtent() * RAISE / 100;
      h.push_back(getFactory()->horizontalSpace(KERN_BEFORE));
      h.push_back(getFactory()->shift(index, shift));
      h.push_back(getFactory()->horizontalSpace(KERN_AFTER));
    }
  h.push_back(getFactory()->shift(rootArea, baseArea->box().height - rootBox.height - KERN));
  h.push_back(baseArea);

  return getFactory()->horizontalArray(h);
}

// XXX does not handle single character vs subformala differently like TeX
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
  const scaled SUB_TOP_MAX = getConstant(context, subscriptTopMax);
  const scaled SUB_GAP_MIN = getConstant(context, subSuperscriptGapMin);
  const scaled SUP_BOT_MIN = getConstant(context, superscriptBottomMin);
  const scaled SUP_BOT_MAX = getConstant(context, superscriptBottomMaxWithSubscript);

  u = std::max(EX, baseBox.height - AXIS);
  v = std::max(AXIS, baseBox.depth + AXIS);

  if (!superScriptBox.defined())
    {
      u = 0;
      v = std::max(v, subScriptBox.height - SUB_TOP_MAX);
    }
  else
    {
      u = std::max(u, superScriptBox.depth + SUP_BOT_MIN);
      if (!subScriptBox.defined())
        {
          v = 0;
        }
      else
        {
          if ((u - superScriptBox.depth) - (subScriptBox.height - v) < SUB_GAP_MIN)
            {
              v = SUB_GAP_MIN - u + superScriptBox.depth + subScriptBox.height;

              scaled psi = SUP_BOT_MAX - (u - superScriptBox.depth);
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

// XXX use italic correction/cut-ins
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

  for (std::vector<AreaRef>::const_iterator p = subScripts.begin(); p != subScripts.end(); p++)
    if (*p) subScriptsBox.append((*p)->box());
  for (std::vector<AreaRef>::const_iterator p = preSubScripts.begin(); p != preSubScripts.end(); p++)
    if (*p) subScriptsBox.append((*p)->box());

  for (std::vector<AreaRef>::const_iterator p = superScripts.begin(); p != superScripts.end(); p++)
    if (*p) superScriptsBox.append((*p)->box());
  for (std::vector<AreaRef>::const_iterator p = preSuperScripts.begin(); p != preSuperScripts.end(); p++)
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
