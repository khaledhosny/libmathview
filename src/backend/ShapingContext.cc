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

#include "Element.hh"
#include "AreaFactory.hh"
#include "ShapingContext.hh"

ShapingContext::ShapingContext(const FormattingContext& c,
			       const UCS4String& src,
			       const std::vector<GlyphSpec>& s,
			       const scaled& v, const scaled& h)
  : m_ctxt(c), m_source(src), m_spec(s), m_vSpan(v), m_hSpan(h), m_index(0)
{ }

unsigned
ShapingContext::chunkSize() const
{
  if (done())
    return 0;
  else
    {
      unsigned n = 1;
      unsigned si = m_spec[m_index].getShaperId();
      unsigned fi = m_spec[m_index].getFontId();
      while (m_index + n < m_spec.size()
	     && m_spec[m_index + n].getShaperId() == si
	     && m_spec[m_index + n].getFontId() == fi) n++;
      return n;
    }
}

unsigned
ShapingContext::getShaperId() const
{
  assert(!done());
  return m_spec[m_index].getShaperId();
}

const GlyphSpec&
ShapingContext::getSpec(int n) const
{
  assert(m_index + n < m_spec.size());
  return m_spec[m_index + n];
}

const Char32*
ShapingContext::data() const
{
  assert(!done());
  return m_source.data() + m_index;
}

AreaRef
ShapingContext::area() const
{
  //

#if 0
    if (m_res.size() == 1) 
      return m_res[0];
    else
#endif

    return getFactory()->glyphString(m_res, m_res_n, m_source);
}

Char32
ShapingContext::prevChar() const
{
  return (m_index > 0) ? m_source[m_index - 1] : 0;
}

Char32
ShapingContext::thisChar() const
{
  return (m_index < m_source.length()) ? m_source[m_index] : 0;
}

Char32
ShapingContext::nextChar() const
{
  return (m_index + 1 < m_source.length()) ? m_source[m_index + 1] : 0;
}

UCS4String
ShapingContext::prevString() const
{
  return m_source.substr(0, m_index);
}

UCS4String
ShapingContext::prevString(UCS4String::size_type l) const
{
  if (l > m_index) l = m_index;
  return m_source.substr(m_index - l, l);
}

UCS4String
ShapingContext::nextString() const
{
  return m_source.substr(m_index, m_source.length() - m_index);
}

UCS4String
ShapingContext::nextString(UCS4String::size_type l) const
{
  if (l > m_source.length() - m_index) l = m_source.length() - m_index;
  return m_source.substr(m_index, l);
}

AreaRef
ShapingContext::popArea(CharIndex& n)
{
  assert(!empty());
  n = m_res_n.back();
  m_res_n.pop_back();
  m_index -= n;
  AreaRef area = m_res.back();
  m_res.pop_back();
  return area;
}

void
ShapingContext::pushArea(CharIndex n, const AreaRef& area)
{
  assert(area);
  assert(m_index + n <= m_source.length());
  m_index += n;
  m_res_n.push_back(n);
  m_res.push_back(area);
}

AreaRef
ShapingContext::getArea(int i) const
{
  if (i >= 0)
    {
      assert(i < m_res.size());
      return m_res[i];
    }
  else
    {
      assert(-i <= m_res.size());
      return m_res[m_res.size() + i];
    }
}
