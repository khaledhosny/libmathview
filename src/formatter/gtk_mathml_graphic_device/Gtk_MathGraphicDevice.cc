/* This file is part of MFD, a Math Formatting Device for various
 * contexts/backends 
 * 
 * Copyright (C) 2003 Luca Padovani <lpadovan@cs.unibo.it>,
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * For more information, please visit the project's home page
 * http://www.cs.unibo.it/~lpadovan/mfd/
 * or send an email to <lpadovan@cs.unibo.it>
 */

#include <config.h>

#include <cassert>

#include "Gtk_MathGraphicDevice.hh"

AreaRef
Gtk_MathGraphicDevice::string(const MathFormattingContext& context,
			      const DOM::GdomeString& str) const
{
  if (context.getElement() == context.getStretchOperator())
    {
    }
  else
    {
    }
}

AreaRef
Gtk_MathGraphicDevice::stretchStringV(const MathFormattingContext& context,
				      const DOM::GdomeString& str,
				      const scaled& height,
				      const scaled& depth) const
{
}

AreaRef
Gtk_MathGraphicDevice::glyph(const MathFormattingContext& context,
			     const DOM::GdomeString& alt,
			     const DOM::GdomeString& fontFamily,
			     unsigned long index) const
{
}

AreaRef
Gtk_MathGraphicDevice::fraction(const MathFormattingContext& context,
				const AreaRef& numerator,
				const AreaRef& denominator,
				const Length& lineThickness) const
{
  std::vector<AreaRef> v(5);
  v.push_back(denominator);
  
  AreaRef s = verticalSpace(context,
			    context.getDisplayStyle() ? defaultLineThickness(context) * 3 : defaultLineThickness(context),
			    scaled::zero());

  v.push_back(s);
  v.push_back(horizontalLine(context, defaultLineThickness(context)));
  v.push_back(s);
  v.push_back(numerator);

  return shift(context, verticalArray(context, v, 2), axis(context));
}

AreaRef
Gtk_MathGraphicDevice::bevelledFraction(const MathFormattingContext& context,
					const AreaRef& numerator,
					const AreaRef& denominator) const
{
  BoundingBox n = numerator->box();
  BoundingBox d = denominator->box();

  std::vector<AreaRef> h(3);
  h.push_back(numerator);
  h.push_back(stretchStringV(context, "/", max(n.height, d.height), max(n.depth, d.depth)));
  h.push_back(denominator);
  
  return horizontalArray(context, h);
}

AreaRef
Gtk_MathGraphicDevice::radical(const MathFormattingContext& context,
			       const AreaRef& radicand,
			       const AreaRef& index) const
{
}

void
Gtk_MathGraphicDevice::calculateScriptShift(const MathFormattingContext& context,
					    const BoundingBox& baseBox,
					    const BoundingBox& subScriptBox,
					    const scaled& subScriptMinShift,
					    const BoundingBox& superScriptBox,
					    const scaled& superScriptMinShift,
					    scaled& v,
					    scaled& u) const
{
  assert(baseBox.defined());

  scaled EX = ex(context);
  scaled AXIS = axis(context);
  scaled RULE = defaultLineThickness(context);

  u = max(EX, baseBox.height - AXIS);
  v = max(AXIS, baseBox.depth - AXIS);

  if (!superScriptBox.defined())
    {
      u = 0;
      v = max(v, max(subScriptMinShift, subScriptBox.height - (EX * 4) / 5));
    }
  else
    {
      u = max(u, max(superScriptMinShift, superScriptBox.depth + EX / 4));
      if (!subScriptBox.defined())
	{
	  v = 0;
	}
      else
	{
	  v = max(v, subScriptMinShift);
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

AreaRef
Gtk_MathGraphicDevice::script(const MathFormattingContext& context,
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
		       evaluate(context, subScriptMinShift, 0),
		       superScript ? superScript->box() : BoundingBox(),
		       evaluate(context, superScriptMinShift, 0),
		       subScriptShift,
		       superScriptShift);

  std::vector<AreaRef> o(2);
  if (subScript) o.push_back(shift(context, subScript, -subScriptShift));
  if (superScript) o.push_back(shift(context, superScript, superScriptShift));

  std::vector<AreaRef> h(2);
  h.push_back(base);
  if (o.size() > 1)
    h.push_back(overlapArray(context, o));
  else
    h.push_back(o[1]);

  return horizontalArray(context, h);
}

AreaRef
Gtk_MathGraphicDevice::multiScripts(const MathFormattingContext& context,
				    const AreaRef& base,
				    const std::vector<AreaRef>& subScripts,
				    const std::vector<AreaRef>& preSubScripts,
				    const Length& subScriptShift,
				    const std::vector<AreaRef>& superScripts,
				    const std::vector<AreaRef>& preSuperScripts,
				    const Length& superScriptShift) const
{
  return base;
}

AreaRef
Gtk_MathGraphicDevice::underOver(const MathFormattingContext& context,
				 const AreaRef& base,
				 const AreaRef& underScript, bool accentUnder,
				 const AreaRef& overScript, bool accent) const
{
  std::vector<AreaRef> v(3);
  if (underScript) v.push_back(center(context, underScript));
  v.push_back(center(context, base));
  if (overScript) v.push_back(center(context, overScript));

  return verticalArray(context, v, underScript ? 1 : 0);
}

AreaRef
Gtk_MathGraphicDevice::longDivision(const MathFormattingContext& context,
				    const AreaRef& base) const
{
  return base;
}

AreaRef
Gtk_MathGraphicDevice::actuarial(const MathFormattingContext& context,
				 const AreaRef& base) const
{
  return base;
}


