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
#include <cstdlib>
#include <assert.h>

#include <sstream>

#include <gdome.h>
#include <gdome-util.h>

#include "defs.h"
#include "gmetadom.hh"

static unsigned
getDepth(const DOM::Element& elem)
{
  unsigned length = 0;
  DOM::Element p = elem;

  while (p)
    {
      p = p.get_parentNode();
      length++;
    }

  return length;
}

static DOM::Element
findCommonAncestor(const DOM::Element& first, const DOM::Element& last)
{
  if (!first || !last) return DOM::Element(0);

  DOM::Element p(first);
  DOM::Element q(last);

  if (p != q)
    {
      unsigned pDepth = getDepth(p);
      unsigned qDepth  = getDepth(q);

      while (p && pDepth > qDepth)
	{
	  p = p.get_parentNode();
	  pDepth--;
	}

      while (q && qDepth > pDepth)
	{
	  q = q.get_parentNode();
	  qDepth--;
	}

      assert(pDepth == qDepth);

      while (p && q && p != q)
	{
	  p = p.get_parentNode();
	  q = q.get_parentNode();
	}
    }
  
  return p;
}

static void
findCommonSiblings(const DOM::Element& first, const DOM::Element& last,
		   DOM::Element& firstS, DOM::Element& lastS)
{
  DOM::Element p(first);
  DOM::Element q(last);

  if (p != q)
    {
      unsigned pDepth = getDepth(p);
      unsigned qDepth  = getDepth(q);

      while (p && pDepth > qDepth)
	{
	  p = p.get_parentNode();
	  pDepth--;
	}

      while (q && qDepth > pDepth)
	{
	  q = q.get_parentNode();
	  qDepth--;
	}

      assert(pDepth == qDepth);

      while (p && q && p.get_parentNode() != q.get_parentNode())
	{
	  p = p.get_parentNode();
	  q = q.get_parentNode();
	}
    }

  firstS = p;
  lastS = q;
}

static DOM::Node
leftmostChild(const DOM::Node& node)
{
  if (!node) return node;

  DOM::Node firstChild = node.get_firstChild();
  if (!firstChild) return node;

  return leftmostChild(firstChild);
}

static DOM::Node
rightmostChild(const DOM::Node& node)
{
  if (!node) return node;

  DOM::Node lastChild = node.get_lastChild();
  if (!lastChild) return node;

  return rightmostChild(lastChild);
}

static DOM::Node
leftSibling(const DOM::Node& node)
{
  DOM::Node p = node;

  if (!p) return p;

  while (p.get_parentNode() && p.get_parentNode().get_firstChild() == p)
    p = p.get_parentNode();

  if (!p.get_parentNode()) return DOM::Node(0);

  DOM::Node prevSibling = p.get_previousSibling();
  assert(prevSibling);

  return rightmostChild(prevSibling);
}

static DOM::Node
rightSibling(const DOM::Node& node)
{
  DOM::Node p = node;

  if (!p) return p;

  DOM::Node firstChild = p.get_firstChild();
  if (firstChild) return firstChild;

  while (p.get_parentNode() && p.get_parentNode().get_lastChild() == p)
    p = p.get_parentNode();

  if (!p.get_parentNode()) return DOM::Node(0);

  DOM::Node nextSibling = p.get_nextSibling();
  assert(nextSibling);

  return leftmostChild(nextSibling);
}

extern "C" GdomeElement*
find_common_ancestor(GdomeElement* first, GdomeElement* last)
{
  DOM::Element p(first);
  DOM::Element q(last);
  return gdome_cast_el(findCommonAncestor(p, q).gdome_object());
}

extern "C" GdomeElement*
find_self_or_ancestor(GdomeElement* elem, const char* uri, const char* name)
{
  DOM::Element el(elem);

  while (el && (el.get_namespaceURI() != uri || el.get_localName() != name))
    el = el.get_parentNode();

  return gdome_cast_el(el.gdome_object());
}

extern "C" GdomeElement*
find_action_element(GdomeElement* elem)
{
  for (DOM::Element el(elem); el; el = el.get_parentNode())
    if ((el.get_namespaceURI() == MATHML_NS_URI && el.get_localName() == "maction") ||
	(el.get_namespaceURI() == BOXML_NS_URI && el.get_localName() == "action"))
      return gdome_cast_el(el.gdome_object());

  return NULL;
}

extern "C" void
action_toggle(GdomeElement* elem)
{
  DOM::Element el(elem);

  guint idx;
  if (el.hasAttribute("selection"))
    idx = atoi(std::string(el.getAttribute("selection")).c_str());
  else idx = 1;

  idx++;

  std::ostringstream os;
  os << idx;
  el.setAttribute("selection", os.str());
}

extern "C" void
find_common_siblings(GdomeElement* first, GdomeElement* last,
		     GdomeElement** firstS, GdomeElement** lastS)
{
  DOM::Element fs(0);
  DOM::Element ls(0);

  findCommonSiblings(DOM::Element(first), DOM::Element(last), fs, ls);

  if (firstS != NULL) *firstS = gdome_cast_el(fs.gdome_object());
  if (lastS != NULL) *lastS = gdome_cast_el(ls.gdome_object());
}

static DOM::Element
findElementWithAttribute(const DOM::Element& elem, const std::string& name)
{
  DOM::Element el(elem);
  while (el && !el.hasAttribute(name)) el = el.get_parentNode();
  return el;
}

static DOM::Element
findElementWithAttributeNS(const DOM::Element& elem, const std::string& ns_uri, const std::string& name)
{
  DOM::Element el(elem);
  while (el && !el.hasAttributeNS(ns_uri, name)) el = el.get_parentNode();
  return el;
}

extern "C" GdomeElement*
find_xref_element(GdomeElement* elem)
{
  DOM::Element el = findElementWithAttribute(DOM::Element(elem), "xref");
  return gdome_cast_el(el.gdome_object());
}

extern "C" GdomeDOMString*
find_hyperlink(GdomeElement* elem, const char* ns_uri, const char* name)
{
  DOM::Element el = findElementWithAttributeNS(DOM::Element(elem), ns_uri, name);
  if (el) return el.getAttributeNS(ns_uri, name).gdome_str();
  else return NULL;
}

extern "C" void
delete_element(GdomeElement* elem)
{
  DOM::Element p(elem);

  DOM::Element parent = p.get_parentNode();
  assert(parent);

  parent.removeChild(p);
}

