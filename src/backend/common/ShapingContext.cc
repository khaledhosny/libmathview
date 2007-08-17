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

#include "Element.hh"
#include "AreaFactory.hh"
#include "ShapingContext.hh"

ShapingContext::ShapingContext(const SmartPtr<Element>& el,
			       const SmartPtr<AreaFactory>& f,
			       const UCS4String& src,
			       const std::vector<GlyphSpec>& s,
			       const scaled& sz,
			       MathVariant mv, bool mm,
			       const scaled& v, const scaled& h)
  : element(el), factory(f), source(src), spec(s), size(sz), mathVariant(mv), mathMode(mm), vSpan(v), hSpan(h), index(0)
{ }

SmartPtr<Element>
ShapingContext::getElement() const
{ return element; }

SmartPtr<AreaFactory>
ShapingContext::getFactory() const
{ return factory; }

unsigned
ShapingContext::chunkSize() const
{
  if (done())
    return 0;
  else
    {
      unsigned n = 1;
      unsigned si = spec[index].getShaperId();
      unsigned fi = spec[index].getFontId();
      while (index + n < spec.size()
	     && spec[index + n].getShaperId() == si
	     && spec[index + n].getFontId() == fi) n++;
      return n;
    }
}

unsigned
ShapingContext::getShaperId() const
{
  assert(!done());
  return spec[index].getShaperId();
}

const GlyphSpec&
ShapingContext::getSpec(int n) const
{
  assert(index + n < spec.size());
  return spec[index + n];
}

const Char32*
ShapingContext::data() const
{
  assert(!done());
  return source.data() + index;
}

AreaRef
ShapingContext::area() const
{
  //

#if 0
    if (res.size() == 1) 
      return res[0];
    else
#endif

    return factory->glyphString(res, res_n, source);
}

Char32
ShapingContext::prevChar() const
{
  return (index > 0) ? source[index - 1] : 0;
}

Char32
ShapingContext::thisChar() const
{
  return (index < source.length()) ? source[index] : 0;
}

Char32
ShapingContext::nextChar() const
{
  return (index + 1 < source.length()) ? source[index + 1] : 0;
}

UCS4String
ShapingContext::prevString() const
{
  return source.substr(0, index);
}

UCS4String
ShapingContext::prevString(UCS4String::size_type l) const
{
  if (l > index) l = index;
  return source.substr(index - l, l);
}

UCS4String
ShapingContext::nextString() const
{
  return source.substr(index, source.length() - index);
}

UCS4String
ShapingContext::nextString(UCS4String::size_type l) const
{
  if (l > source.length() - index) l = source.length() - index;
  return source.substr(index, l);
}

AreaRef
ShapingContext::popArea(CharIndex& n)
{
  assert(!empty());
  n = res_n.back();
  res_n.pop_back();
  index -= n;
  AreaRef area = res.back();
  res.pop_back();
  return area;
}

void
ShapingContext::pushArea(CharIndex n, const AreaRef& area)
{
  assert(area);
  assert(index + n <= source.length());
  index += n;
  res_n.push_back(n);
  res.push_back(area);
}

AreaRef
ShapingContext::getArea(int i) const
{
  if (i >= 0)
    {
      assert(i < res.size());
      return res[i];
    }
  else
    {
      assert(-i <= res.size());
      return res[res.size() + i];
    }
}
