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

MathGraphicDevice::MathGraphicDevice()
  : shaperManager(ShaperManager::create())
{ }

MathGraphicDevice::~MathGraphicDevice()
{
  shaperManager->unregisterShapers();
}

double
MathGraphicDevice::dpi(const MathFormattingContext&) const
{
  return 72.0;
}

scaled
MathGraphicDevice::evaluate(const MathFormattingContext& context,
			    const Length& length, const scaled& defaultValue) const
{
  switch (length.type)
    {
    case Length::UNDEFINED_UNIT:
      return defaultValue;
    case Length::PURE_UNIT:
      // error?
      return defaultValue * length.value;
    case Length::INFINITY_UNIT:
      return scaled::max();
    case Length::LT_UNIT:
      return defaultLineThickness(context) * length.value;
    case Length::EM_UNIT:
      return em(context) * length.value;
    case Length::EX_UNIT:
      return ex(context) * length.value;
    case Length::PX_UNIT:
      return scaled((72.27 * length.value) / dpi(context));
    case Length::IN_UNIT:
      return scaled(72.27 * length.value);
    case Length::CM_UNIT:
      return scaled(72.27 * (length.value / 2.54));
    case Length::MM_UNIT:
      return scaled(72.27 * (length.value / 25.4));
    case Length::PT_UNIT:
      return scaled(length.value);
    case Length::PC_UNIT:
      return scaled(12 * 72.27 * length.value);
    case Length::PERCENTAGE_UNIT:
      return (defaultValue * length.value) / 100.0;
    default:
      if (length.type >= Length::NEGATIVE_VERYVERYTHICK_SPACE &&
	  length.type <= Length::VERYVERYTHICK_SPACE)
	{
	  Length newLength =
	    context.getMathSpace(MathFormattingContext::NEGATIVE_VERYVERYTHICK_SPACE +
				 length.type - Length::NEGATIVE_VERYVERYTHICK_SPACE);
	  assert(newLength.type < Length::NEGATIVE_VERYVERYTHICK_SPACE ||
		 newLength.type > Length::VERYVERYTHICK_SPACE);
	  return evaluate(context, newLength, defaultValue);
	}
      else
	{
	  assert(false);
	  return defaultValue;
	}
    }
}

scaled
MathGraphicDevice::em(const MathFormattingContext& context) const
{
  return context.getSize();
}

scaled
MathGraphicDevice::ex(const MathFormattingContext& context) const
{
  return string(context, "x")->box().height;
}

scaled
MathGraphicDevice::axis(const MathFormattingContext& context) const
{
  const BoundingBox pbox = string(context, "+")->box();
  // the + is a better choice rather than x because its vertical extent
  // is certainly an odd number of pixels, whereas the x has almost
  // certainly an even number of pixels. This way it is reduced the
  // approximation due to the / 2  
  return (pbox.height - pbox.depth) / 2;
}

scaled
MathGraphicDevice::defaultLineThickness(const MathFormattingContext& context) const
{
  // at least 1px thick
  return std::max(context.getSize() / 10, scaled(72.27 / dpi(context)));
}

AreaRef
MathGraphicDevice::wrapper(const MathFormattingContext&, const AreaRef& area) const
{
  return factory->box(area, area->box());
}

AreaRef
MathGraphicDevice::dummy(const MathFormattingContext& context) const
{
  return getFactory()->color(string(context, StringOfUCS4String(UCS4String(1, 0xfffd))), RGBColor::RED());
}

AreaRef
MathGraphicDevice::string(const MathFormattingContext& context,
			  const String& str) const
{
  if (str.length() == 0)
    return dummy(context);
  else
    {
      UCS4String source = UCS4StringOfString(str);
      mapMathVariant(context.getVariant(), source);

      if (context.getElement() == context.getStretchOperator())
	return getShaperManager()->shapeStretchy(context, source,
						 context.getStretchV(),
						 context.getStretchH());
      else
	return getShaperManager()->shape(context, source);
    }
}

AreaRef
MathGraphicDevice::stretchStringV(const MathFormattingContext& context,
				  const String& str,
				  const scaled& height,
				  const scaled& depth) const
{
  return getShaperManager()->shapeStretchy(context, UCS4StringOfString(str), height, depth);
}

AreaRef
MathGraphicDevice::glyph(const MathFormattingContext& context,
			 const String& alt,
			 const String& fontFamily,
			 unsigned long index) const
{
  assert(false);
}

AreaRef
MathGraphicDevice::fraction(const MathFormattingContext& context,
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
MathGraphicDevice::bevelledFraction(const MathFormattingContext& context,
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
MathGraphicDevice::radical(const MathFormattingContext& context,
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
MathGraphicDevice::calculateDefaultScriptShift(const MathFormattingContext& context,
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
MathGraphicDevice::calculateScriptShift(const MathFormattingContext& context,
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
MathGraphicDevice::script(const MathFormattingContext& context,
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
MathGraphicDevice::multiScripts(const MathFormattingContext& context,
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
MathGraphicDevice::underOver(const MathFormattingContext& context,
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
MathGraphicDevice::enclose(const MathFormattingContext& context,
			   const AreaRef& base,
			   const String& notation) const
{
  return base;
}
