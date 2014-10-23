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

#include "Configuration.hh"
#include "ValueConversion.hh"

template <typename Model>
struct TemplateSetup
{
  static String
  appendSectionName(const String& path, const String& name)
  {
    if (path.empty())
      return name;
    else
      return path + "/" + name;
  }

  static void
  parse(const AbstractLogger& logger, Configuration& conf, const typename Model::Element& node, const String& path)
  {
    for (typename Model::ElementIterator iter(node); iter.more(); iter.next())
      {
	typename Model::Element elem = iter.element();
	assert(elem);
	const String name = Model::getNodeName(Model::asNode(elem));

	if (name == "section")
	  {
	    const String sectionName = Model::getAttribute(elem, "name");
	    parse(logger, conf, elem, appendSectionName(path, sectionName));
	  }
	else if (name == "key")
	  {
	    const String keyName = Model::getAttribute(elem, "name");
	    const String keyValue = Model::getElementValue(elem);
	    conf.add(appendSectionName(path, keyName), keyValue);
	  }
	else
	  logger.out(LOG_WARNING, "unrecognized element `%s' in configuration file (ignored)", name.c_str());
      } 
  }

  static void
  parse(const AbstractLogger& logger, Configuration& conf, const typename Model::Element& node)
  { parse(logger, conf, node, ""); }

  template <class Class, bool subst>
  static bool
  load(const AbstractLogger& logger, Class& obj, const String& description, const String& rootTag, const String& path)
  {
    logger.out(LOG_DEBUG, "loading %s from `%s'...", description.c_str(), path.c_str());
    if (typename Model::Document doc = Model::document(logger, path, subst))
      {
	bool res = true;
	if (typename Model::Element root = Model::getDocumentElement(doc))
          {
	  if (Model::getNodeName(Model::asNode(root)) == rootTag)
	    parse(logger, obj, root);
	  else 
	    {
	      logger.out(LOG_WARNING, "configuration file `%s': could not find root element", path.c_str());
	      res = false;
	    }
          }
	Model::freeDocument(doc);
	return res;
      }
    return false;
  }
};

