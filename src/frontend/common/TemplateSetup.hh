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

#include "Configuration.hh"
#include "ValueConversion.hh"
#include "Attribute.hh"
#include "AttributeSet.hh"
#include "AttributeSignature.hh"
#include "MathMLAttributeSignatures.hh"
#include "MathMLOperatorDictionary.hh"

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

  static void
  getAttribute(const typename Model::Element& node, const AttributeSignature& signature,
	       const SmartPtr<AttributeSet>& aList)
  {
    assert(aList);

    const String attrVal = Model::getAttribute(node, signature.name);
    if (attrVal.empty()) return;

    aList->set(Attribute::create(signature, attrVal));
  }

  static void
  parse(const AbstractLogger& logger, MathMLOperatorDictionary& dictionary, const typename Model::Element& root)
  {
    for (typename Model::ElementIterator iter(root, "*", "operator"); iter.more(); iter.next())
      {
	typename Model::Element elem = iter.element();
	const String opName = Model::getAttribute(elem, "name");
      
	if (!opName.empty())
	  {
	    SmartPtr<AttributeSet> defaults = AttributeSet::create();
	  
	    getAttribute(elem, ATTRIBUTE_SIGNATURE(MathML, Operator, form), defaults);
	    getAttribute(elem, ATTRIBUTE_SIGNATURE(MathML, Operator, fence), defaults);
	    getAttribute(elem, ATTRIBUTE_SIGNATURE(MathML, Operator, separator), defaults);
	    getAttribute(elem, ATTRIBUTE_SIGNATURE(MathML, Operator, lspace), defaults);
	    getAttribute(elem, ATTRIBUTE_SIGNATURE(MathML, Operator, rspace), defaults);
	    getAttribute(elem, ATTRIBUTE_SIGNATURE(MathML, Operator, stretchy), defaults);
	    getAttribute(elem, ATTRIBUTE_SIGNATURE(MathML, Operator, symmetric), defaults);
	    getAttribute(elem, ATTRIBUTE_SIGNATURE(MathML, Operator, maxsize), defaults);
	    getAttribute(elem, ATTRIBUTE_SIGNATURE(MathML, Operator, minsize), defaults);
	    getAttribute(elem, ATTRIBUTE_SIGNATURE(MathML, Operator, largeop), defaults);
	    getAttribute(elem, ATTRIBUTE_SIGNATURE(MathML, Operator, movablelimits), defaults);
	    getAttribute(elem, ATTRIBUTE_SIGNATURE(MathML, Operator, accent), defaults);
	  
	    dictionary.add(logger, opName, Model::getAttribute(elem, "form"), defaults);
	  } 
	else
	  logger.out(LOG_WARNING, "operator dictionary: could not find operator name");
      }
  }

  template <class Class, bool subst>
  static bool
  load(const AbstractLogger& logger, Class& obj, const String& description, const String& rootTag, const String& path)
  {
    logger.out(LOG_DEBUG, "loading %s from `%s'...", description.c_str(), path.c_str());
    if (typename Model::Document doc = Model::document(logger, path, subst))
      if (typename Model::Element root = Model::getDocumentElement(doc))
	if (Model::getNodeName(Model::asNode(root)) == rootTag)
	  {
	    parse(logger, obj, root);
	    return true;
	  }
	else 
	  logger.out(LOG_WARNING, "configuration file `%s': could not find root element", path.c_str());
    return false;
  }
};

