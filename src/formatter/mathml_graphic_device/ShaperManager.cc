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

#include "Shaper.hh"
#include "ShaperManager.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"

ShaperManager::ShaperManager() : nextShaperId(0)
{
  for (unsigned i = 0; i < MAX_SHAPERS; i++)
    shaper[i] = 0;
}

ShaperManager::~ShaperManager()
{
  for (unsigned i = 0; i < nextShaperId; i++)
    shaper[i]->unregisterShaper(this, i);
}

AreaRef
ShaperManager::shapeAux(const MathFormattingContext& ctxt, ShapingResult& result) const
{
  while (!result.done())
    {
      if (getShaper(result.getShaperId())->shape(ctxt, result) == 0)
	{
	  // this is very severe, a Shaper has declared it is able to handle
	  // a character but it turned out that it has eaten no characters from
	  // the result! This may make sense for example if there is a sort of
	  // fallback mechanism and a given shaper can "refuse" to process a
	  // character in some context
	  assert(false);
	  return 0;
	}
    }

  return result.area(ctxt.getDevice()->getFactory());
}

AreaRef
ShaperManager::shape(const MathFormattingContext& ctxt, const DOM::UCS4String& source) const
{
  std::vector<GlyphSpec> spec;
  spec.reserve(source.length());
  for (unsigned i = 0; i < source.length(); i++)
    spec.push_back(map(source[i]));
  ShapingResult result(source, spec);
  return shapeAux(ctxt, result);
}

AreaRef
ShaperManager::shapeStretchy(const MathFormattingContext& ctxt,
			     const DOM::UCS4String& source,
			     const scaled& vSpan,
			     const scaled& hSpan) const
{
  std::vector<GlyphSpec> spec(source.length());
  for (unsigned i = 0; i < source.length(); i++)
    spec.push_back(mapStretchy(source[i]));
  ShapingResult result(source, spec, vSpan, hSpan);
  return shapeAux(ctxt, result);
}

unsigned
ShaperManager::registerShaper(const SmartPtr<Shaper>& s)
{
  assert(s);
  assert(nextShaperId < MAX_SHAPERS);
  unsigned shaperId = nextShaperId++;
  shaper[shaperId] = s;
  s->registerShaper(this, shaperId);
  return shaperId;
}

GlyphSpec
ShaperManager::registerChar(DOM::Char32 ch, const GlyphSpec& spec)
{
  assert(ch <= BIGGEST_CHAR);
  GlyphSpec oldSpec = glyphSpec[ch];
  //printf("registering %x (old spec shaper id was %d new is %d)\n", ch, oldSpec.getShaperId(), spec.getShaperId());
  glyphSpec.set(ch, spec);
  return oldSpec;
}

GlyphSpec
ShaperManager::registerStretchyChar(DOM::Char32 ch, const GlyphSpec& spec)
{
  assert(ch <= BIGGEST_CHAR);
  GlyphSpec oldSpec = glyphSpec[ch];
  glyphSpec.set(ch | STRETCHY_FLAG, spec);
  return oldSpec;
}

GlyphSpec
ShaperManager::map(DOM::Char32 ch) const
{
  assert(ch <= BIGGEST_CHAR);
  return glyphSpec[ch];
}

GlyphSpec
ShaperManager::mapStretchy(DOM::Char32 ch) const
{
  assert(ch <= BIGGEST_CHAR);
  GlyphSpec spec = glyphSpec[ch | STRETCHY_FLAG];
  return spec.getShaperId() ? spec : map(ch);
}

SmartPtr<class Shaper>
ShaperManager::getShaper(unsigned si) const
{
  assert(si < MAX_SHAPERS);
  assert(shaper[si]);
  return shaper[si];
}
