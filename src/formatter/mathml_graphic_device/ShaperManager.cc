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

ShaperManager::ShaperManager() : nextShaperId(0)
{
  for (unsigned i = 0; i < MAX_SHAPERS; i++)
    shaper[i] = 0;
}

AreaRef
ShaperManager::shapeAux(ShapingResult& result) const
{
  while (!result.done())
    {
      if (getShaper(result.getShaperId()).shape(result) == 0)
	return 0;
    }

  return result.area();
}

AreaRef
ShaperManager::shape(const SmartPtr<AreaFactory>& factory,
		     const DOM::UCS4String& source,
		     const scaled& fontSize) const
{
  std::vector<GlyphSpec> spec;
  spec.reserve(source.length());
  for (unsigned i = 0; i < source.length(); i++)
    spec.push_back(map(source[i]));
  ShapingResult result(factory, source, spec, fontSize);
  return shapeAux(result);
}

AreaRef
ShaperManager::shapeStretchy(const SmartPtr<AreaFactory>& factory,
			     const DOM::UCS4String& source,
			     const scaled& fontSize,
			     const scaled& vSpan,
			     const scaled& hSpan) const
{
  std::vector<GlyphSpec> spec(source.length());
  for (unsigned i = 0; i < source.length(); i++)
    spec.push_back(mapStretchy(source[i]));
  ShapingResult result(factory, source, spec, fontSize, vSpan, hSpan);
  return shapeAux(result);
}

unsigned
ShaperManager::registerShaper(const Shaper& s)
{
  assert(nextShaperId < MAX_SHAPERS);
  unsigned shaperId = nextShaperId++;
  shaper[shaperId] = &s;
  s.registerChars(*this, shaperId);
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

const class Shaper&
ShaperManager::getShaper(unsigned si) const
{
  assert(si < MAX_SHAPERS);
  assert(shaper[si]);
  return *shaper[si];
}
