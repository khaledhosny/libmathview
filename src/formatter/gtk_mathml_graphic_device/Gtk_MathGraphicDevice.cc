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

#include "Gtk_AreaFactory.hh"
#include "Gtk_MathGraphicDevice.hh"
#include "Gtk_DefaultPangoShaper.hh"
#include "Gtk_PangoShaper.hh"
#include "Gtk_AdobeShaper.hh"
#include "MathVariantMap.hh"
#include "SpaceShaper.hh"
#include "MathMLElement.hh"

Gtk_MathGraphicDevice::Gtk_MathGraphicDevice(GtkWidget* widget)
{
  factory = Gtk_AreaFactory::create();

  GObjectPtr<PangoContext> context = gtk_widget_create_pango_context(widget);
  SmartPtr<Gtk_DefaultPangoShaper> defaultPangoShaper = Gtk_DefaultPangoShaper::create();
  defaultPangoShaper->setPangoContext(context);
  getShaperManager()->registerShaper(defaultPangoShaper);

  getShaperManager()->registerShaper(SpaceShaper::create());

  SmartPtr<Gtk_PangoShaper> pangoShaper = Gtk_PangoShaper::create();
  pangoShaper->setPangoContext(context);
  getShaperManager()->registerShaper(pangoShaper);

  //getShaperManager()->registerShaper(Gtk_AdobeShaper::create());
}

Gtk_MathGraphicDevice::~Gtk_MathGraphicDevice()
{ }

SmartPtr<AreaFactory>
Gtk_MathGraphicDevice::getFactory() const
{
  return factory;
}

AreaRef
Gtk_MathGraphicDevice::string(const MathFormattingContext& context,
			      const String& str) const
{
  DOM::UCS4String source = toUCS4String(str);
  mapMathVariant(context.getVariant(), source);

  if (context.getElement() == context.getStretchOperator())
    return getShaperManager()->shapeStretchy(context, source,
					     context.getStretchV(),
					     context.getStretchH());
  else
    return getShaperManager()->shape(context, source);
}

AreaRef
Gtk_MathGraphicDevice::stretchStringV(const MathFormattingContext& context,
				      const String& str,
				      const scaled& height,
				      const scaled& depth) const
{
  return getShaperManager()->shapeStretchy(context, toUCS4String(str), height, depth);
}

AreaRef
Gtk_MathGraphicDevice::glyph(const MathFormattingContext& context,
			     const String& alt,
			     const String& fontFamily,
			     unsigned long index) const
{
  assert(false);
}

AreaRef
Gtk_MathGraphicDevice::fraction(const MathFormattingContext& context,
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
Gtk_MathGraphicDevice::bevelledFraction(const MathFormattingContext& context,
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
Gtk_MathGraphicDevice::radical(const MathFormattingContext& context,
			       const AreaRef& base,
			       const AreaRef& index) const
{
  const scaled RULE = defaultLineThickness(context);
  DOM::UCS4String root(1, 0x221a);
  BoundingBox baseBox = base->box();
  AreaRef rootArea = stretchStringV(context, fromUCS4String(root), baseBox.depth, baseBox.height + RULE);
  
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
Gtk_MathGraphicDevice::calculateDefaultScriptShift(const MathFormattingContext& context,
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
Gtk_MathGraphicDevice::calculateScriptShift(const MathFormattingContext& context,
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
    h.push_back(o[0]);

  return factory->horizontalArray(h);
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
  std::vector<AreaRef> v;
  v.reserve(3);
  if (underScript) v.push_back(factory->center(underScript));
  v.push_back(factory->center(base));
  if (overScript) v.push_back(factory->center(overScript));

  return factory->verticalArray(v, underScript ? 1 : 0);
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

AreaRef
Gtk_MathGraphicDevice::enclose(const MathFormattingContext& context,
			       const AreaRef& base,
			       const String& notation) const
{
  return base;
}

AreaRef
Gtk_MathGraphicDevice::wrapper(const MathFormattingContext& context,
			       const AreaRef& base) const
{
  BoundingBox box = base->box();
  return factory->wrapper(base->fit(box.width, box.height, box.depth), box, context.getElement());
}
