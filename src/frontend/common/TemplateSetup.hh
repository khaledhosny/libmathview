// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget/, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#include <cassert>

#include "Globals.hh"
#include "Configuration.hh"
#include "ValueConversion.hh"
#include "Attribute.hh"
#include "AttributeList.hh"
#include "AttributeSignature.hh"
#include "MathMLAttributeSignatures.hh"
#include "MathMLOperatorDictionary.hh"
#include "MathMLEntitiesTable.hh"

template <typename Model>
struct TemplateSetup
{
  static bool
  parseColor(const typename Model::Element& node, RGBColor& f, RGBColor& b)
  {
    String fs = node.getAttribute("foreground");
    String bs = node.getAttribute("background");
    
    if (fs.empty() || bs.empty())
      {
	String s_name = node.get_nodeName();
	Globals::logger(LOG_WARNING, "malformed `%s' element in configuration file", s_name.c_str());
	return false;
      }

    SmartPtr<Value> fv = ATTRIBUTE_SIGNATURE(MathML, Token, mathcolor).parseValue(fs);
    SmartPtr<Value> bv = ATTRIBUTE_SIGNATURE(MathML, Token, mathbackground).parseValue(bs);

    if (!fv || !bv)
      {
	String s_name = node.get_nodeName();
	Globals::logger(LOG_WARNING, "malformed color attribute in configuration file, `%s' element", s_name.c_str());
	return false;
      }

    f = ToRGB(fv);
    b.transparent = ToTokenId(bv) == T_TRANSPARENT;
    if (!b.transparent) b = ToRGB(bv);

    return true;
  }

  static void
  parse(Configuration& conf, const typename Model::Element& node)
  {
    for (typename Model::ElementIterator iter(node); iter.more(); iter.next())
      {
	typename Model::Element elem = iter.element();
	assert(elem);
	std::string name = elem.get_nodeName();
    
	if (name == "dictionary-path")
	  {
	    std::string path = Model::elementValue(elem);
	    if (!path.empty())
	      {
		Globals::logger(LOG_DEBUG, "found dictionary path `%s'", path.c_str());
		conf.addDictionary(path);
	      }
	  } 
	else if (name == "font-size")
	  {
	    std::string attr = elem.getAttribute("size");
	    if (attr.empty())
	      Globals::logger(LOG_WARNING, "malformed `font-size' element, cannot find `size' attribute");
	    else
	      {
		conf.setFontSize(atoi(attr.c_str()));
		Globals::logger(LOG_DEBUG, "default font size set to %d points", conf.getFontSize());
	      }
	  } 
	else if (name == "color")
	  {
	    RGBColor f;
	    RGBColor b;
	    if (parseColor(elem, f, b))
	      {
		conf.setForeground(f); 
		conf.setBackground(b);
	      }
	  }
	else if (name == "link-color")
	  {
	    RGBColor f;
	    RGBColor b;
	    if (parseColor(elem, f, b))
	      {
		conf.setLinkForeground(f);
		conf.setLinkBackground(b);
	      }
	  }
	else if (name == "select-color")
	  {
	    RGBColor f;
	    RGBColor b;
	    if (parseColor(elem, f, b))
	      {
		conf.setSelectForeground(f);
		conf.setSelectBackground(b);
	      }
	  }
	else
	  Globals::logger(LOG_WARNING, "unrecognized element `%s' in configuration file (ignored)", name.c_str());
      } 
  }

  static void
  getAttribute(const typename Model::Element& node, const AttributeSignature& signature,
	       const SmartPtr<AttributeList>& aList)
  {
    assert(aList);

    String attrVal = node.getAttribute(signature.name);
    if (attrVal.empty()) return;

    aList->set(Attribute::create(signature, attrVal));
  }

  static void
  parse(MathMLOperatorDictionary& dictionary, const typename Model::Element& root)
  {
    for (typename Model::ElementIterator iter(root, "*", "operator"); iter.more(); iter.next())
      {
	typename Model::Element elem = iter.element();
	String opName = elem.getAttribute("name");
      
	if (!opName.empty())
	  {
	    SmartPtr<AttributeList> defaults = AttributeList::create();
	  
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
	  
	    dictionary.add(opName, elem.getAttribute("form"), defaults);
	  } 
	else
	  Globals::logger(LOG_WARNING, "operator dictionary: could not find operator name");
      }
  }

  template <class Class, bool subst>
  static bool
  load(Class& obj, const String& description, const String& rootTag, const String& path)
  {
    Globals::logger(LOG_DEBUG, "loading %s from `%s'...", description.c_str(), path.c_str());
    if (typename Model::Element root = typename Model::ElementIterator(Model::parseXML(path, subst), "*", rootTag).element())
      {
	parse(obj, root);
	return true;
      }
    else
      {
	Globals::logger(LOG_WARNING, "configuration file `%s': could not find root element", path.c_str());
	return false;
      }
  }
};

