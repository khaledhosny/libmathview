// Copyright (C) 2000-2002, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://www.cs.unibo.it/helm/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#include <cassert>

#include "ChildList.hh"

#if defined(HAVE_GMETADOM)

ChildList::ChildList(const DOM::Element& root0,
		     const DOM::GdomeString& ns0,
		     const DOM::GdomeString& name0)
  : root(root0), ns(ns0), name(name0)
{
  assert(root);
}

DOM::Node
ChildList::item(unsigned long i) const
{
  DOM::Node p = root.get_firstChild();

  while (p)
    {
      if ((ns == "*" || ns == p.get_namespaceURI()) &&
	  (name == "*" || p.get_localName() == name))
	{
	  if (i == 0) break;
	  i--;
	}

      p = p.get_nextSibling();
    }

  return p;
}

unsigned long
ChildList::get_length() const
{
  unsigned long length = 0;
  DOM::Node p = root.get_firstChild();

  while (p)
    {
      if ((ns == "*" || ns == p.get_namespaceURI()) &&
	  (name == "*" || p.get_localName() == name))
	length++;
      p = p.get_nextSibling();
    }

  return length;
}

#endif // HAVE_GMETADOM
