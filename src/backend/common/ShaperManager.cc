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

#include "AbstractLogger.hh"
#include "Shaper.hh"
#include "NullShaper.hh"
#include "ShapingContext.hh"
#include "ShaperManager.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "Element.hh"
#include "MathMLElement.hh"
#include "Area.hh"
#include "GlyphArea.hh"

ShaperManager::ShaperManager(const SmartPtr<AbstractLogger>& l)
  : nextShaperId(0), logger(l), errorShaper(smart_cast<Shaper>(NullShaper::create(l)))
{
  for (unsigned i = 0; i < MAX_SHAPERS; i++)
    shaper[i] = 0;
}

ShaperManager::~ShaperManager()
{ }

SmartPtr<ShaperManager>
ShaperManager::create(const SmartPtr<AbstractLogger>& logger)
{ return new ShaperManager(logger); }

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
		     const SmartPtr<Element>& elem,
		     const SmartPtr<AreaFactory>& factory,
		     const UCS4String& source) const
{
  std::vector<GlyphSpec> spec;
  spec.reserve(source.length());
  for (unsigned i = 0; i < source.length(); i++)
    spec.push_back(map(source[i]));
  ShapingContext context(elem, factory, source, spec,
			 ctxt.getSize(), ctxt.getVariant(), ctxt.getMathMode());
  return shapeAux(context);
}

AreaRef
ShaperManager::shapeStretchy(const FormattingContext& ctxt,
			     const SmartPtr<Element>& elem,
			     const SmartPtr<AreaFactory>& factory,
			     const UCS4String& source,
			     const scaled& vSpan,
			     const scaled& hSpan) const
{
  std::vector<GlyphSpec> spec;
  spec.reserve(source.length());
  for (unsigned i = 0; i < source.length(); i++)
    spec.push_back(mapStretchy(source[i]));
  ShapingContext context(elem, factory, source, spec,
			 ctxt.getSize(), ctxt.getVariant(), ctxt.getMathMode(), vSpan, hSpan);
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
ShaperManager::registerChar(Char32 ch, const GlyphSpec& spec)
{
  assert(ch <= BIGGEST_CHAR);
  GlyphSpec oldSpec = glyphSpec[ch];
  //printf("registering %x (old spec shaper id was %d new is %d)\n", ch, oldSpec.getShaperId(), spec.getShaperId());
  glyphSpec.set(ch, spec);
  return oldSpec;
}

GlyphSpec
ShaperManager::registerStretchyChar(Char32 ch, const GlyphSpec& spec)
{
  assert(ch <= BIGGEST_CHAR);
  GlyphSpec oldSpec = glyphSpec[ch];
  //printf("registering stretchy %x (old spec shaper id was %d new is %d)\n", ch, oldSpec.getShaperId(), spec.getShaperId());
  glyphSpec.set(ch | STRETCHY_FLAG, spec);
  return oldSpec;
}

const GlyphSpec&
ShaperManager::map(Char32 ch) const
{
  assert(ch <= BIGGEST_CHAR);
  return glyphSpec[ch];
}

const GlyphSpec&
ShaperManager::mapStretchy(Char32 ch) const
{
  assert(ch <= BIGGEST_CHAR);
  return glyphSpec[ch | STRETCHY_FLAG];
}

SmartPtr<class Shaper>
ShaperManager::getShaper(unsigned si) const
{ return (si < nextShaperId) ? shaper[si] : 0; }

AreaRef
ShaperManager::compose(const FormattingContext& context,
		       const AreaRef base, const UCS4String baseSource,
	       	       const AreaRef script, const UCS4String scriptSource,
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

  const AreaRef over = overScript ? script : NULL;
  const AreaRef under = overScript ? NULL : script;
   
  return context.MGD()->getFactory()->combinedGlyph(base, over, under,
					    	    dx, dy, dxUnder);
}
