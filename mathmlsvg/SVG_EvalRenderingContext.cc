// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include "Model.hh"
#include "Location.hh"
#include "SVG_EvalRenderingContext.hh"

SVG_EvalRenderingContext::SVG_EvalRenderingContext(const SmartPtr<AbstractLogger>& logger,
						   const SmartPtr<MathView>& _view)
  : SVG_RenderingContext(logger), view(_view)
{ }

SVG_EvalRenderingContext::~SVG_EvalRenderingContext()
{ }

#include <iostream>

void
SVG_EvalRenderingContext::wrapperStart(const scaled& x, const scaled& y, const BoundingBox& box,
				       const SmartPtr<Element>& elem)
{
  std::cerr << "SVG_EvalRenderingContext::wrapperStart IN" << std::endl;
  assert(elem);
  if (xmlElement* el = view->modelElementOfElement(elem))
    {
      if (xmlChar* id = xmlGetProp((xmlNode*) el, Model::toModelString("id")))
	{
	  const String name = Model::fromModelString(id);
	  std::cerr << "FOUND ID = " << name << std::endl;
	  idMap[name] = Location::create(name, x, y, box);
	  xmlFree(id);
	}
    }
  std::cerr << "SVG_EvalRenderingContext::wrapperStart OUT" << std::endl;
}

void
SVG_EvalRenderingContext::wrapperEnd()
{
  std::cerr << "SVG_EvalRenderingContext::wrapperEnd" << std::endl;
}

SmartPtr<Location>
SVG_EvalRenderingContext::getLocation(const String& name) const
{
  const IdMap::const_iterator p = idMap.find(name);
  if (p != idMap.end())
    return p->second;
  else
    return 0;
}

