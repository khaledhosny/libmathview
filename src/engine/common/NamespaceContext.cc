// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#include <config.h>

#include "View.hh"
#include "NamespaceContext.hh"
#include "Element.hh"

NamespaceContext::NamespaceContext(const String& ns,
				   const SmartPtr<View>& v)
  : namespaceURI(ns), view(v)
{ }

NamespaceContext::~NamespaceContext()
{ }

SmartPtr<View>
NamespaceContext::getView() const
{
  return static_cast<View*>(view);
}

void
NamespaceContext::setDefaultFontSize(unsigned size)
{
  assert(size > 0);
  if (defaultFontSize != size)
    {
      defaultFontSize = size;
      if (SmartPtr<Element> elem = getView()->getRootElement())
	{
	  elem->setDirtyAttributeD();
	  elem->setDirtyLayout();	  
	}
    }
}

