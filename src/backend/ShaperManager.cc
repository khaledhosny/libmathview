// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
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

#include "Shaper.hh"
#include "NullShaper.hh"
#include "ShapingContext.hh"
#include "ShaperManager.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "MathVariantMap.hh"
#include "Element.hh"
#include "MathMLElement.hh"
#include "Area.hh"
#include "GlyphArea.hh"

ShaperManager::ShaperManager(void)
  : nextShaperId(0)
  , errorShaper(nullptr)
{
  errorShaper = NullShaper::create();
  for (auto & elem : shaper)
    elem = nullptr;
}

ShaperManager::~ShaperManager()
{ }

SmartPtr<ShaperManager>
ShaperManager::create(void)
{ return new ShaperManager(); }

AreaRef
ShaperManager::shapeAux(ShapingContext& context) const
{
  while (!context.done())
    {
      const unsigned index = context.getIndex();
      if (SmartPtr<Shaper> shaper = getShaper(context.getShaperId()))
	shaper->shape(context);
      if (index == context.getIndex())
	{
	  // this is very severe, either no shaper was configured, or
	  // a Shaper has declared it is able to handle a character
	  // but it turned out that it has eaten no characters from
	  // the context! This would make sense for example if there
	  // were a sort of fallback mechanism and a given shaper
	  // could "refuse" to process a character in some context,
	  // but unfortunately there is not!
	  errorShaper->shape(context);
	  // the default shapper cannot fail
	  assert(index != context.getIndex());
	}
    }

  return context.area();
}

AreaRef
ShaperManager::shape(const FormattingContext& ctxt,
                     const std::string& str) const
{
  std::u32string source = UCS4StringOfString(str);
#if 0
  // XXX: We might want to handle math variant differently for text mode, e.g.
  // by using font styles instead so that we get proper text kerning and so on.
  if (ctxt.getMathMode())
#endif
    mapMathVariant(ctxt.getVariant(), source);

  std::vector<GlyphSpec> spec;
  spec.reserve(source.length());
  for (auto & elem : source)
    spec.push_back(map(elem));
  ShapingContext context(ctxt, source, spec);
  return shapeAux(context);
}

AreaRef
ShaperManager::shapeStretchy(const FormattingContext& ctxt,
			     const std::string& str,
			     const scaled& vSpan,
			     const scaled& hSpan) const
{
  std::u32string source = UCS4StringOfString(str);
#if 0
  // XXX: does it make sense to do math variant mapping for stretchy
  // characters?
  if (ctxt.getMathMode())
    mapMathVariant(ctxt.getVariant(), source);
#endif

  std::vector<GlyphSpec> spec;
  spec.reserve(source.length());
  for (auto & elem : source)
    spec.push_back(mapStretchy(elem));
  ShapingContext context(ctxt, source, spec, vSpan, hSpan);
  return shapeAux(context);
}

unsigned
ShaperManager::registerShaper(const SmartPtr<Shaper>& s)
{
  assert(s);
  assert(nextShaperId < MAX_SHAPERS);
  unsigned shaperId = nextShaperId++;
  // if the shaper is not capable of handling every character we
  // cannot assign it the index 0, because this index is the default
  // for characters that have not been explicitly registered.
  // In the worst case the error shaper will be used as fallback.
  if (shaperId == 0 && !s->isDefaultShaper())
    shaperId = nextShaperId++;
  shaper[shaperId] = s;
  s->registerShaper(this, shaperId);
  return shaperId;
}

void
ShaperManager::unregisterShapers()
{
  // WARNING: this code was in the destructor, but this results in a loop.
  // if we are in the destructor the ref counter of the shaper manager is 0
  // Now, if we call unregisterShaper then the ref counter gets incremented
  // and then decremented (the parameter is a smart pointer) resulting in
  // another call to the destructor
  for (unsigned i = 0; i < nextShaperId; i++)
    if (shaper[i])
      shaper[i]->unregisterShaper(this, i);
}

GlyphSpec
ShaperManager::registerChar(char32_t ch, const GlyphSpec& spec)
{
  assert(ch <= BIGGEST_CHAR);
  GlyphSpec oldSpec = glyphSpec[ch];
  //printf("registering %x (old spec shaper id was %d new is %d)\n", ch, oldSpec.getShaperId(), spec.getShaperId());
  glyphSpec.set(ch, spec);
  return oldSpec;
}

GlyphSpec
ShaperManager::registerStretchyChar(char32_t ch, const GlyphSpec& spec)
{
  assert(ch <= BIGGEST_CHAR);
  GlyphSpec oldSpec = glyphSpec[ch];
  //printf("registering stretchy %x (old spec shaper id was %d new is %d)\n", ch, oldSpec.getShaperId(), spec.getShaperId());
  glyphSpec.set(ch | STRETCHY_FLAG, spec);
  return oldSpec;
}

const GlyphSpec&
ShaperManager::map(char32_t ch) const
{
  assert(ch <= BIGGEST_CHAR);
  return glyphSpec[ch];
}

const GlyphSpec&
ShaperManager::mapStretchy(char32_t ch) const
{
  assert(ch <= BIGGEST_CHAR);
  return glyphSpec[ch | STRETCHY_FLAG];
}

SmartPtr<class Shaper>
ShaperManager::getShaper(unsigned si) const
{ return (si < nextShaperId) ? shaper[si] : nullptr; }

AreaRef
ShaperManager::compose(const FormattingContext& context,
		       const AreaRef base, const std::u32string baseSource,
		       const AreaRef script, const std::u32string scriptSource,
		       bool overScript)
{
  scaled dx = scaled::zero();
  scaled dy = scaled::zero();
  scaled dxUnder= scaled::zero();

  const GlyphSpec& baseGlyphSpec = map(baseSource[0]);
  const GlyphSpec& scriptGlyphSpec = map(scriptSource[0]);
  //we control if the base char and the combining char are render
  //to the same Shaper
  if (baseGlyphSpec.getShaperId() == scriptGlyphSpec.getShaperId())
  {
    if (overScript)
      shaper[baseGlyphSpec.getShaperId()]->computeCombiningCharOffsetsAbove(base,
   					               	     	     	    script,
								            dx, dy);
    else
      shaper[baseGlyphSpec.getShaperId()]->computeCombiningCharOffsetsBelow(base,
   					               	     	       	    script,
								            dxUnder);
  } 
  //we define a default values of dx and dy 
  else
  {
    dx = (base->box().width + script->box().width)/ 2;
    dy = base->box().height + script->box().depth;
  }

  const AreaRef over = overScript ? script : nullptr;
  const AreaRef under = overScript ? nullptr : script;
   
  return context.MGD()->getFactory()->combinedGlyph(base, over, under,
					    	    dx, dy, dxUnder);
}
