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

#include <sstream>
#include <iomanip>
#include <iostream>
#include "AbstractLogger.hh"
#include "libxml2_Model.hh"
#include "libxml2_MathView.hh"
#include "SVG_libxml2_StreamRenderingContext.hh"

SVG_libxml2_StreamRenderingContext::SVG_libxml2_StreamRenderingContext(const SmartPtr<AbstractLogger>& logger,
								       std::ostream& output,
								       const SmartPtr<libxml2_MathView>& _view)
  : SVG_StreamRenderingContext(logger, output), view(_view)
{ }

SVG_libxml2_StreamRenderingContext::~SVG_libxml2_StreamRenderingContext()
{ }

String
SVG_libxml2_StreamRenderingContext::toSVGLength(const scaled& s) const
{
  std::ostringstream buffer;
  buffer << std::fixed << std::setprecision(2) << (s.toFloat() * 72.27) / 90;
  return buffer.str();
}

String
SVG_libxml2_StreamRenderingContext::getId(const SmartPtr<Element>& elem) const
{
  assert(elem);
  if (xmlElement* el = view->modelElementOfElement(elem))
    {
      if (xmlChar* id = xmlGetProp((xmlNode*) el, libxml2_Model::toModelString("id")))
	{
	  String res = libxml2_Model::fromModelString(id);
	  xmlFree(id);
	  return res;
	}
    }
  return "";
}
