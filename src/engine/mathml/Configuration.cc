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

#include "Globals.hh"
#include "Configuration.hh"
#include "MathMLParseFile.hh"
#include "ValueConversion.hh"
#include "AttributeSignature.hh"
#include "MathMLAttributeSignatures.hh"

Configuration::Configuration(void)
{
  colorSet = linkColorSet = selectColorSet = false;
}

Configuration::~Configuration()
{
}

#if defined(HAVE_GMETADOM)

bool
Configuration::Load(const char* confPath)
{
  assert(confPath != NULL);

  Globals::logger(LOG_DEBUG, "loading configuration from `%s'...", confPath);

  try {
    DOM::Document doc = MathMLParseFile(confPath, false);

    DOM::Element root = doc.get_documentElement();
    if (!root) {
      Globals::logger(LOG_WARNING, "configuration file `%s' has no root node", confPath);
      return false;
    }

    if (root.get_nodeName() != "math-engine-configuration") {
      Globals::logger(LOG_WARNING, "configuration file `%s': could not find root element", confPath);
      return false;
    }

    ParseConfiguration(root);

    return true;
  } catch (DOM::DOMException) {
    return false;
  }
}

void
Configuration::ParseConfiguration(const DOM::Element& node)
{
  for (DOM::Node p = node.get_firstChild(); p; p = p.get_nextSibling()) {
    if (p.get_nodeType() == DOM::Node::ELEMENT_NODE) {
      DOM::Element elem(p);
      std::string name = elem.get_nodeName();
    
      if (name == "dictionary-path") {
	std::string path = elementValue(elem);
	if (!path.empty()) {
	  Globals::logger(LOG_DEBUG, "found dictionary path `%s'", path.c_str());
	  dictionaries.push_back(path);
	}
      } else if (name == "entities-table-path") {
	std::string path = elementValue(elem);
	if (!path.empty()) {
	  Globals::logger(LOG_DEBUG, "found entities table path `%s'", path.c_str());
	  entities.push_back(path);
	}
      } else if (name == "t1-config-path") {
	std::string path = elementValue(elem);
	if (!path.empty() && t1Configs.empty()) {
	  Globals::logger(LOG_DEBUG, "found t1lib config path `%s'", path.c_str());
	  t1Configs.push_back(path);
	}
      } else if (name == "font-size") {
	std::string attr = elem.getAttribute("size");
	if (attr.empty()) {
	  Globals::logger(LOG_WARNING, "malformed `font-size' element, cannot find `size' attribute");
	} else {
	  fontSize = atoi(attr.c_str());
	  Globals::logger(LOG_DEBUG, "default font size set to %d points", fontSize);
	  fontSizeSet = true;
	}
      } else if (name == "color")
	colorSet = ParseColor(elem, foreground, background);
      else if (name == "link-color")
	linkColorSet = ParseColor(elem, linkForeground, linkBackground, transparentLinkBackground);
      else if (name == "select-color")
	selectColorSet = ParseColor(elem, selectForeground, selectBackground);
      else {
	Globals::logger(LOG_WARNING, "unrecognized element `%s' in configuration file (ignored)", name.c_str());
      }
    } else if (!DOM::nodeIsBlank(p)) {
      Globals::logger(LOG_WARNING, "unrecognized node type `%d' in configuration file (ignored)", p.get_nodeType());
    }
  }
}

bool
Configuration::ParseColor(const DOM::Element& node, RGBColor& f, RGBColor& b)
{
  bool unused;
  return ParseColor(node, f, b, unused);
}

bool
Configuration::ParseColor(const DOM::Element& node, RGBColor& f, RGBColor& b, bool& transparent)
{
  String fs = fromDOMString(node.getAttribute("foreground"));
  String bs = fromDOMString(node.getAttribute("background"));

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
  transparent = ToTokenId(bv) == T_TRANSPARENT;
  if (!transparent) b = ToRGB(bv);

  return true;
}

#endif // HAVE_GMETADOM
