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

#include "ShapingResult.hh"

unsigned
ShapingResult::chunkSize() const
{
  assert(!done());
  unsigned n = 1;
  unsigned si = getShaperId();
  while (index + n < spec.size() && spec[index + n].getShaperId() == si) n++;
  return n;
}

unsigned
ShapingResult::getShaperId() const
{
  assert(!done());
  return spec[index].getShaperId();
}

GlyphSpec
ShapingResult::getSpec(int n) const
{
  assert(index + n < spec.size());
  return spec[index + n];
}

const DOM::Char32*
ShapingResult::data() const
{
  assert(!done());
  return source.data() + index;
}

AreaRef
ShapingResult::area(const SmartPtr<AreaFactory>& factory) const
{
#if 0
  if (res.size() == source.length())
#endif
    if (res.size() == 1) 
      return res[0];
    else
      return factory->horizontalArray(res);
#if 0
  else
    {
      std::vector<AreaRef> content;
      content.reserve(source.length());
      AreaRef empty = factory->horizontalSpace(scaled::zero());
      for (unsigned i = 0; i < res.size(); i++)
	{
	  for (unsigned j = 0; j < res_n[i] - 1; j++)
	    content.push_back(empty);
	  content.push_back(res[i]);
	}
      return factory->horizontalArray(content);
    }
#endif
}

DOM::Char32
ShapingResult::prevChar() const
{
  return (index > 0) ? source[index - 1] : 0;
}

DOM::Char32
ShapingResult::thisChar() const
{
  return (index < source.length()) ? source[index] : 0;
}

DOM::Char32
ShapingResult::nextChar() const
{
  return (index + 1 < source.length()) ? source[index + 1] : 0;
}

DOM::UCS4String
ShapingResult::prevString(int l) const
{
  if (l < 0 || l > index) l = index;
  return source.substr(index - l, l);
}

DOM::UCS4String
ShapingResult::nextString(int l) const
{
  if (l < 0 || l > source.length() - index) l = source.length() - index;
  return source.substr(index, l);
}

AreaRef
ShapingResult::popArea(unsigned& n)
{
  assert(!empty());
  n = res_n.back();
  res_n.pop_back();
  AreaRef area = res.back();
  res.pop_back();
  return area;
}

void
ShapingResult::pushArea(unsigned n, const AreaRef& area)
{
  assert(area);
  assert(index + n >= 0 && index + n <= source.length());
  index += n;
  res_n.push_back(n);
  res.push_back(area);
}
