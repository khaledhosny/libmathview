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
  assert(elem);
  if (xmlElement* el = view->modelElementOfElement(elem))
    {
      if (xmlChar* id = xmlGetProp((xmlNode*) el, Model::toModelString("id")))
	{
	  const String name = Model::fromModelString(id);
	  idMap[name] = Location::create(name, x, y, box);
	  xmlFree(id);



	}
    }
}

void
SVG_EvalRenderingContext::wrapperEnd()
{
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

