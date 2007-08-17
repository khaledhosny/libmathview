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

#include "CombinedGlyphArea.hh"
#include "ContainerArea.hh"
#include "Rectangle.hh"
#include "GlyphArea.hh"
#include "GlyphStringArea.hh"

CombinedGlyphArea::CombinedGlyphArea(const AreaRef& b, const AreaRef& a, const AreaRef& u,
		  		     const scaled& skewchar, const scaled& verticalSpace,
				     const scaled& underSkew)
  : ContainerArea(), base(b), accent(a), under(u), dx(skewchar), dy(verticalSpace),
    dxUnder(underSkew)
{
  scaled width = base->box().width;
  scaled depth = under ? under->box().depth + base->box().depth : base->box().depth;
  scaled height = accent ? dy + accent->box().height : base->box().height;

  bbox = BoundingBox(width, height, depth); 
}

CombinedGlyphArea::~CombinedGlyphArea()
{  }

BoundingBox
CombinedGlyphArea::box() const
{
  return bbox;
}

void
CombinedGlyphArea::render(class RenderingContext& rCxtx, const scaled& x, const scaled& y) const
{
  if (accent)
    accent->render(rCxtx, x + dx, y + dy);
  if (under)
    under->render(rCxtx, x + dxUnder, y);

  base->render(rCxtx, x, y);
}

AreaRef 
CombinedGlyphArea::fit(const scaled& width, const scaled& height, const scaled& depth) const
{
  return this;
}

scaled
CombinedGlyphArea::leftEdge() const
{
  return scaled::min(); // FIXME
}

scaled 
CombinedGlyphArea::rightEdge() const
{
  return std::max(base->box().width, dx + accent->box().width);
}

void
CombinedGlyphArea::strength(int& w, int& h, int& d) const
{
  // I apply this method to an Area without children
  w = h = d = 0;
}

AreaRef 
CombinedGlyphArea::node(AreaIndex) const
{
  // cannot have children
  assert(false); 
}

void
CombinedGlyphArea::origin(AreaIndex, class Point&) const
{
  // cannot have children
  assert(false);
}

CharIndex 
CombinedGlyphArea::lengthTo(AreaIndex) const
{ 
  // cannot have children 
  assert(false);
}

bool 
CombinedGlyphArea::searchByArea(class AreaId&, const AreaRef& area) const
{
  if (base == area)
    return true;
  else if (accent && accent == area)
    return true;
  else if (under && under == area)
   return true;
  else
    return false;
}

bool 
CombinedGlyphArea::searchByCoords(class AreaId& id, const scaled& x, const scaled& y) const
{
  return Rectangle(scaled::zero(), scaled::zero(), bbox).isInside(x, y);
}

bool 
CombinedGlyphArea::searchByIndex(class AreaId&, CharIndex) const
{
  return false;
}

bool 
CombinedGlyphArea::indexOfPosition(const scaled& x, const scaled& y, CharIndex& index) const
{
  if (Rectangle(scaled::zero(), scaled::zero(), bbox).isInside(x, y))
  {
    index = (x < bbox.width / 2) ? 0 : length();
    return true;
  }
  else
   return false;
}

bool 
CombinedGlyphArea::positionOfIndex(CharIndex index, class Point*, BoundingBox* b) const
{
  if (index == 0 && index < length())
  {
    if (b) *b = bbox;
    return true;
  }
  else
    return false;
}

SmartPtr<const GlyphArea>
CombinedGlyphArea::getGlyphArea() const
{
  return base->getGlyphArea();
}

SmartPtr<const GlyphStringArea>
CombinedGlyphArea::getGlyphStringArea(void) const
{
  return base->getGlyphStringArea();
}
