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

#include "AreaFactory.hh"
#include "ShapingContext.hh"
#include "MathGraphicDevice.hh"
#include "MathMLFractionElement.hh"
#include "MathMLIdentifierElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLRowElement.hh"
#include "ShaperManager.hh"
#include "SpaceShaper.hh"
#include "traverseAux.hh"

struct FixedSpaceData
{
  Char16 ch;
  int space;
} fixedSpaceMap[] = {
  { 0x0009, 9 * 8 }, // Tab
  { 0x0020,  9 }, // Space
  { 0x00a0,  9 }, // NonBreakingSpace
  { 0x2000,  9 }, // EN QUAD
  { 0x2001, 18 }, // EM QUAD
  { 0x2002,  9 }, // EN SPACE (HALF AN EM)
  { 0x2003, 18 }, // EM SPACE 
  { 0x2004,  6 }, // THREE-PER-EM SPACE (THICK SPACE)
  { 0x2005,  4 }, // FOUR-PER-EM SPACE (MID SPACE)
  { 0x2006,  3 }, // SIX-PER-EM SPACE (THIN SPACE)
  { 0x2009,  3 }, // THIN SPACE
  { 0x200a,  1 }, // HAIR SPACE (VERY THIN SPACE)
  { 0x200b,  0 }, // ZERO-WIDTH SPACE
  { 0x205f,  4 }, // MEDIUM MATH SPACE

  { 0x0000,  0 }
};

Char16 csSpaceMap[] = {
  0x2061, // FUNCTION APPLICATION
  0x2062, // INVISIBLE TIMES
  0x2063, // INVISIBLE SEPARATOR

  0x0000
};

#define FIXED_SPACE_MAP_INDEX 0
#define CS_SPACE_MAP_INDEX 1

void
SpaceShaper::registerShaper(const SmartPtr<ShaperManager>& sm, unsigned shaperId)
{
  assert(sm);

  for (unsigned i = 0; fixedSpaceMap[i].ch; i++)
    sm->registerChar(fixedSpaceMap[i].ch, GlyphSpec(shaperId, FIXED_SPACE_MAP_INDEX, i));

  for (unsigned i = 0; csSpaceMap[i]; i++)
    sm->registerChar(csSpaceMap[i], GlyphSpec(shaperId, CS_SPACE_MAP_INDEX, i));
}

void
SpaceShaper::unregisterShaper(const SmartPtr<ShaperManager>&, unsigned)
{
  // nothing to do???
}

void
SpaceShaper::shape(ShapingContext& context) const
{
  assert(!context.done());

  GlyphSpec spec = context.getSpec();
  switch (spec.getFontId())
    {
    case FIXED_SPACE_MAP_INDEX:
      shapeFixedSpace(context, spec);
      break;
    case CS_SPACE_MAP_INDEX:
      shapeContextSensitiveSpace(context, spec);
      break;
    default:
      assert(false);
    }
}

void
SpaceShaper::pushSpace(ShapingContext& context, int space, unsigned n)
{
  assert(n > 0);
  context.pushArea(n, context.getFactory()->horizontalSpace(context.getSize() * space / 18));
}

void
SpaceShaper::shapeFixedSpace(ShapingContext& context, const GlyphSpec& spec)
{
  unsigned n = 1;
  int space = fixedSpaceMap[spec.getGlyphId()].space;
  if (context.nextChar() == 0xfe00)
    {
      space = -space;
      n++;
    }
  pushSpace(context, space, n);
}

void
SpaceShaper::shapeContextSensitiveSpace(ShapingContext& context, const GlyphSpec& spec)
{
  int space = 0;
  switch (context.thisChar())
    {
    case 0x2061: // FUNCTION APPLICATION
      space = shapeFunctionApplication(context);
      break;
    case 0x2062: // INVISIBLE TIMES
      space = shapeInvisibleTimes(context);
      break;
    case 0x2063: // INVISIBLE SEPARATOR
      space = 0;
      break;
    default:
      assert(false);
      break;
    }
  pushSpace(context, space);
}

int
SpaceShaper::shapeFunctionApplication(const class ShapingContext& context)
{
  if (SmartPtr<MathMLOperatorElement> op = smart_cast<MathMLOperatorElement>(context.getElement()))
    {
      SmartPtr<MathMLElement> next = findRightSibling(op);
      if (!next) return 0;

      if (SmartPtr<MathMLOperatorElement> coreOp = next->getCoreOperatorTop())
	if (coreOp->IsFence()) return 0;

      if (SmartPtr<MathMLRowElement> row = smart_cast<MathMLRowElement>(next))
	if (SmartPtr<MathMLOperatorElement> coreOp = smart_cast<MathMLOperatorElement>(row->getChild(0)))
	  if (coreOp->IsFence()) return 0;

      return 5;
    }

  return 0;
}

int
SpaceShaper::shapeInvisibleTimes(const ShapingContext& context)
{
  // THESE CONSTANTS SHOULD BE CHECKED ON SOME MANUAL
  if (SmartPtr<MathMLOperatorElement> op = smart_cast<MathMLOperatorElement>(context.getElement()))
    {
      SmartPtr<MathMLElement> prev = findLeftSibling(op);
      SmartPtr<MathMLElement> next = findRightSibling(op);
      if (!prev || !next) return 0;

      if (is_a<MathMLIdentifierElement>(prev) && is_a<MathMLIdentifierElement>(next))
	{
	  SmartPtr<MathMLTokenElement> prevToken = smart_cast<MathMLTokenElement>(prev);
	  SmartPtr<MathMLTokenElement> nextToken = smart_cast<MathMLTokenElement>(next);
	  assert(prevToken && nextToken);
    
	  if (prevToken->GetLogicalContentLength() <= 1 &&
	      nextToken->GetLogicalContentLength() <= 1) return 0;

	  return 4;
	} 
      else if (is_a<MathMLIdentifierElement>(prev))
	return 5;
      else if (is_a<MathMLFractionElement>(prev) && is_a<MathMLFractionElement>(next))
	return 5;
      else if (is_a<MathMLFractionElement>(prev) || is_a<MathMLFractionElement>(next))
	return 4;
    }

  return 0;
}
