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

#include <functional>
#include <algorithm>

#include <cassert>

#include "gmetadom_Setup.hh"
#include "gmetadomIterators.hh"
#include "Globals.hh"
#include "Configuration.hh"
#include "ValueConversion.hh"
#include "Attribute.hh"
#include "AttributeList.hh"
#include "AttributeSignature.hh"
#include "MathMLAttributeSignatures.hh"
#include "MathMLOperatorDictionary.hh"
#include "MathMLEntitiesTable.hh"

DOM::Document
parseXMLFile(const char* filename, bool subst)
{
  if (!subst)
    {
      DOM::DOMImplementation di;
      return di.createDocumentFromURI(filename);
    } 
  else
    {
      GdomeDOMImplementation* di = gdome_di_mkref();
      assert(di != NULL);
      GdomeException exc = 0;
      GdomeDocument* doc = gdome_di_createDocFromURIWithEntitiesTable(di,
								      filename,
								      getMathMLEntities(),
								      GDOME_LOAD_PARSING | GDOME_LOAD_SUBSTITUTE_ENTITIES,
								      &exc);
      if (exc != 0)
	{
	  gdome_di_unref(di, &exc);
	  gdome_doc_unref(doc, &exc);
	  return DOM::Document(0);
	}

      if (doc == 0)
	{
	  // FIXME: this should be signalled as an exception, I think
	  gdome_di_unref(di, &exc);
	  return DOM::Document(0);
	}

      DOM::Document res(doc);
      gdome_di_unref(di, &exc);
      assert(exc == 0);
      gdome_doc_unref(doc, &exc);
      assert(exc == 0);
    
      return res;
    }
}

static bool
parseColor(const DOM::Element& node, RGBColor& f, RGBColor& b)
{
  String fs = DOMX::fromDOMString(node.getAttribute("foreground"));
  String bs = DOMX::fromDOMString(node.getAttribute("background"));

  if (fs.empty() || bs.empty()) {
    std::string s_name = node.get_nodeName();
    Globals::logger(LOG_WARNING, "malformed `%s' element in configuration file", s_name.c_str());
    return false;
  }

  SmartPtr<Value> fv = ATTRIBUTE_SIGNATURE(MathML, Token, mathcolor).parseValue(fs);
  SmartPtr<Value> bv = ATTRIBUTE_SIGNATURE(MathML, Token, mathbackground).parseValue(bs);

  if (!fv || !bv)
    {
      std::string s_name = node.get_nodeName();
      Globals::logger(LOG_WARNING, "malformed color attribute in configuration file, `%s' element", s_name.c_str());
      return false;
    }

  f = ToRGB(fv);
  b.transparent = ToTokenId(bv) == T_TRANSPARENT;
  if (!b.transparent) b = ToRGB(bv);

  return true;
}

static void
parseConfiguration(Configuration& conf, const DOM::Element& node)
{
  DOMX::ChildElementsIterator iter(node);
  while (DOM::Element elem = iter.element())
    {
      std::string name = elem.get_nodeName();
    
      if (name == "dictionary-path")
	{
	  std::string path = DOMX::elementValue(elem);
	  if (!path.empty()) {
	    Globals::logger(LOG_DEBUG, "found dictionary path `%s'", path.c_str());
	    conf.addDictionary(path);
	  }
	} 
      else if (name == "font-size")
	{
	  std::string attr = elem.getAttribute("size");
	  if (attr.empty()) {
	    Globals::logger(LOG_WARNING, "malformed `font-size' element, cannot find `size' attribute");
	  }
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
	{
	  Globals::logger(LOG_WARNING, "unrecognized element `%s' in configuration file (ignored)", name.c_str());
	}
    } 
}

bool
loadConfiguration(Configuration& conf, const char* confPath)
{
  assert(confPath != NULL);

  Globals::logger(LOG_DEBUG, "loading configuration from `%s'...", confPath);

  try
    {
      DOM::Document doc = parseXMLFile(confPath);

      DOM::Element root = doc.get_documentElement();
      if (!root)
	{
	  Globals::logger(LOG_WARNING, "configuration file `%s' has no root node", confPath);
	  return false;
	}

      if (root.get_nodeName() != "math-engine-configuration")
	{
	  Globals::logger(LOG_WARNING, "configuration file `%s': could not find root element", confPath);
	  return false;
	}

      parseConfiguration(conf, root);

      return true;
    }
  catch (DOM::DOMException)
    {
      return false;
    }
}

static void
getAttribute(const DOM::Element& node, const AttributeSignature& signature,
	     const SmartPtr<AttributeList>& aList)
{
  assert(aList);

  DOM::GdomeString attrVal = node.getAttribute(DOMX::toDOMString(signature.name));
  if (attrVal.empty()) return;

  aList->set(Attribute::create(signature, DOMX::fromDOMString(attrVal)));
}

bool
loadOperatorDictionary(MathMLOperatorDictionary& dictionary, const char* fileName)
{
  try
    {
      DOM::Document doc = parseXMLFile(fileName, true);

      DOM::Element root = doc.get_documentElement();
      if (!root)
	{
	  Globals::logger(LOG_WARNING, "operator dictionary `%s': parse error", fileName);
	  return false;
	}

      if (root.get_nodeName() != "dictionary")
	{
	  Globals::logger(LOG_WARNING, "operator dictionary `%s': could not find root element", fileName);
	  return false;
	}

      DOMX::ChildElementsIterator iter(root, "*", "operator");
      while (DOM::Element elem = iter.element())
	{
	  String opName = DOMX::fromDOMString(elem.getAttribute("name"));

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
	    Globals::logger(LOG_WARNING, "operator dictionary `%s': could not find operator name", fileName);
	}
      return true;
    } 
  catch (DOM::DOMException)
    {
      return false;
    }
}

