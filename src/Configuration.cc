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

#include <assert.h>
#include <stdlib.h>

#include "Globals.hh"
#include "StringUnicode.hh"
#include "Configuration.hh"
#include "AttributeParser.hh"
#include "MathMLParseFile.hh"
#include "ValueConversion.hh"

// definition of local adaptors
struct DeleteStringAdaptor
  : public std::unary_function<String*,void>
{
  void operator()(String* s) const
  { delete s; }
};

Configuration::Configuration(void)
{
  colorSet = linkColorSet = selectColorSet = false;
}

Configuration::~Configuration()
{
  std::for_each(dictionaries.begin(), dictionaries.end(), DeleteStringAdaptor());
  std::for_each(entities.begin(), entities.end(), DeleteStringAdaptor());
  std::for_each(fonts.begin(), fonts.end(), DeleteStringAdaptor());
  std::for_each(t1Configs.begin(), t1Configs.end(), DeleteStringAdaptor());
}

#if defined(HAVE_MINIDOM)

bool
Configuration::Load(const char* confPath)
{
  assert(confPath != NULL);

  Globals::logger(LOG_DEBUG, "loading configuration from `%s'...", confPath);

  mDOMDocRef doc = MathMLParseFile(confPath, false);
  if (doc == NULL) return false;

  mDOMNodeRef root = mdom_doc_get_root_node(doc);
  if (root == NULL) {
    mdom_doc_free(doc);
    Globals::logger(LOG_WARNING, "configuration file `%s' has no root node", confPath);
    return false;
  }

  if (!mdom_string_eq(mdom_node_get_name(root), DOM_CONST_STRING("math-engine-configuration"))) {
    mdom_doc_free(doc);
    Globals::logger(LOG_WARNING, "configuration file `%s': could not find root element", confPath);
    return false;
  }

  ParseConfiguration(root);

  mdom_doc_free(doc);

  return true;
}

void
Configuration::ParseConfiguration(mDOMNodeRef node)
{
  assert(node != NULL);
  
  for (mDOMNodeRef p = mdom_node_get_first_child(node);
       p != NULL;
       p = mdom_node_get_next_sibling(p)) {
    mDOMConstStringRef name = mdom_node_get_name(p);
    
    if (mdom_string_eq(name, DOM_CONST_STRING("dictionary-path"))) {
      mDOMStringRef path = mdom_node_get_content(p);
      if (path != NULL) {
	Globals::logger(LOG_DEBUG, "found dictionary path `%s'", C_STRING(path));
	String* s = new StringC(C_STRING(path));
	s->TrimSpacesLeft();
	s->TrimSpacesRight();
	dictionaries.push_back(s);
      }
    } else if (mdom_string_eq(name, DOM_CONST_STRING("font-configuration-path"))) {
      mDOMStringRef path = mdom_node_get_content(p);
      if (path != NULL) {
	Globals::logger(LOG_DEBUG, "found font configuration path `%s'", C_STRING(path));
	String* s = new StringC(C_STRING(path));
	s->TrimSpacesLeft();
	s->TrimSpacesRight();
	fonts.push_back(s);
      }
    } else if (mdom_string_eq(name, DOM_CONST_STRING("entities-table-path"))) {
      mDOMStringRef path = mdom_node_get_content(p);
      if (path != NULL) {
	Globals::logger(LOG_DEBUG, "found entities table path `%s'", C_STRING(path));
	String* s = new StringC(C_STRING(path));
	s->TrimSpacesLeft();
	s->TrimSpacesRight();
	entities.push_back(s);
      }
    } else if (mdom_string_eq(name, DOM_CONST_STRING("t1-config-path"))) {
      mDOMStringRef path = mdom_node_get_content(p);
      if (path != NULL && t1Configs.empty()) {
	Globals::logger(LOG_DEBUG, "found t1lib config path `%s'", C_STRING(path));
	String* s = new StringC(C_STRING(path));
	s->TrimSpacesLeft();
	s->TrimSpacesRight();
	t1Configs.push_back(s);
      }
    } else if (mdom_string_eq(name, DOM_CONST_STRING("font-size"))) {
      mDOMStringRef attr = mdom_node_get_attribute(p, DOM_CONST_STRING("size"));
      if (attr == NULL) {
	Globals::logger(LOG_WARNING, "malformed `font-size' element, cannot find `size' attribute");
      } else {
	fontSize = atoi(C_STRING(attr));
	Globals::logger(LOG_DEBUG, "default font size set to %d points", fontSize);
	fontSizeSet = true;
      }
    } else if (mdom_string_eq(name, DOM_CONST_STRING("color"))) {
      colorSet = ParseColor(p, foreground, background);
      if (colorSet) Globals::logger(LOG_DEBUG, "default color set to %06x %06x", foreground, background);
      else Globals::logger(LOG_WARNING, "color parsing error in configuration file");
    } else if (mdom_string_eq(name, DOM_CONST_STRING("link-color"))) {
      linkColorSet = ParseColor(p, linkForeground, linkBackground);
      if (linkColorSet) Globals::logger(LOG_DEBUG, "default link color set to %06x %06x", linkForeground, linkBackground);
      else Globals::logger(LOG_WARNING, "color parsing error in configuration file");
    } else if (mdom_string_eq(name, DOM_CONST_STRING("select-color"))) {
      selectColorSet = ParseColor(p, selectForeground, selectBackground);
      if (selectColorSet) Globals::logger(LOG_DEBUG, "default selection color set to %06x %06x", selectForeground, selectBackground);
      else Globals::logger(LOG_WARNING, "color parsing error in configuration file");
    } else if (!mdom_node_is_blank(p)) {
      Globals::logger(LOG_WARNING, "unrecognized entry %s in configuration file (ignored)",
			 C_STRING(mdom_node_get_name(p)));
    }
  }
}

bool
Configuration::ParseColor(mDOMNodeRef node, RGBValue& f, RGBValue& b)
{
  assert(node != NULL);

  mDOMStringRef fs = mdom_node_get_attribute(node, DOM_STRING("foreground"));
  mDOMStringRef bs = mdom_node_get_attribute(node, DOM_STRING("background"));

  if (fs == NULL || bs == NULL) {
    Globals::logger(LOG_WARNING, "malformed `%s' element in configuration file",
		       mdom_node_get_name(node));
    return false;
  }

  StringC fss(C_STRING(fs));
  StringC bss(C_STRING(bs));

  StringTokenizer fst(fss);
  StringTokenizer bst(bss);

  const Value* fv = colorParser(fst);
  const Value* bv = colorParser(bst);

  if (fv == NULL || bv == NULL) {
    delete fv;
    delete bv;

    Globals::logger(LOG_WARNING, "malformed color attribute in configuration file, `%s' element",
		       mdom_node_get_name(node));

    return false;
  }

  f = ToRGB(fv);
  b = ToRGB(bv);

  delete fv;
  delete bv;

  return true;
}

#elif defined(HAVE_GMETADOM)

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
      DOM::GdomeString name = elem.get_nodeName();
    
      if (name == "dictionary-path") {
	DOM::GdomeString path = elementValue(elem);
	if (!path.empty()) {
	  std::string s_path = path;
	  Globals::logger(LOG_DEBUG, "found dictionary path `%s'", s_path.c_str());
	  String* s = new StringC(s_path.c_str());
	  s->TrimSpacesLeft();
	  s->TrimSpacesRight();
	  dictionaries.push_back(s);
	}
      } else if (name == "font-configuration-path") {
	DOM::GdomeString path = elementValue(elem);
	if (!path.empty()) {
	  std::string s_path = path;
	  Globals::logger(LOG_DEBUG, "found font configuration path `%s'", s_path.c_str());
	  String* s = new StringC(s_path.c_str());
	  s->TrimSpacesLeft();
	  s->TrimSpacesRight();
	  fonts.push_back(s);
	}
      } else if (name == "entities-table-path") {
	DOM::GdomeString path = elementValue(elem);
	if (!path.empty()) {
	  std::string s_path = path;
	  Globals::logger(LOG_DEBUG, "found entities table path `%s'", s_path.c_str());
	  String* s = new StringC(s_path.c_str());
	  s->TrimSpacesLeft();
	  s->TrimSpacesRight();
	  entities.push_back(s);
	}
      } else if (name == "t1-config-path") {
	DOM::GdomeString path = elementValue(elem);
	if (!path.empty() && t1Configs.empty()) {
	  std::string s_path = path;
	  Globals::logger(LOG_DEBUG, "found t1lib config path `%s'", s_path.c_str());
	  String* s = new StringC(s_path.c_str());
	  s->TrimSpacesLeft();
	  s->TrimSpacesRight();
	  t1Configs.push_back(s);
	}
      } else if (name == "font-size") {
	DOM::GdomeString attr = elem.getAttribute("size");
	if (attr.empty()) {
	  Globals::logger(LOG_WARNING, "malformed `font-size' element, cannot find `size' attribute");
	} else {
	  std::string s_attr = attr;
	  fontSize = atoi(s_attr.c_str());
	  Globals::logger(LOG_DEBUG, "default font size set to %d points", fontSize);
	  fontSizeSet = true;
	}
      } else if (name == "color") {
	colorSet = ParseColor(elem, foreground, background);
	if (colorSet) Globals::logger(LOG_DEBUG, "default color set to %06x %06x", foreground, background);
	else Globals::logger(LOG_WARNING, "color parsing error in configuration file");
      } else if (name == "link-color") {
	linkColorSet = ParseColor(elem, linkForeground, linkBackground);
	if (linkColorSet) Globals::logger(LOG_DEBUG, "default link color set to %06x %06x", linkForeground, linkBackground);
	else Globals::logger(LOG_WARNING, "color parsing error in configuration file");
      } else if (name == "select-color") {
	selectColorSet = ParseColor(elem, selectForeground, selectBackground);
	if (selectColorSet) Globals::logger(LOG_DEBUG, "default selection color set to %06x %06x", selectForeground, selectBackground);
	else Globals::logger(LOG_WARNING, "color parsing error in configuration file");
      } else {
	std::string s_name = name;
	Globals::logger(LOG_WARNING, "unrecognized element `%s' in configuration file (ignored)", s_name.c_str());
      }
    } else if (!DOM::nodeIsBlank(p)) {
      Globals::logger(LOG_WARNING, "unrecognized node type `%d' in configuration file (ignored)", p.get_nodeType());
    }
  }
}

bool
Configuration::ParseColor(const DOM::Element& node, RGBValue& f, RGBValue& b)
{
  DOM::GdomeString fs = node.getAttribute("foreground");
  DOM::GdomeString bs = node.getAttribute("background");

  if (fs.empty() || bs.empty()) {
    std::string s_name = node.get_nodeName();
    Globals::logger(LOG_WARNING, "malformed `%s' element in configuration file", s_name.c_str());
    return false;
  }

  std::string s_fs = fs;
  std::string s_bs = bs;
  StringC fss(s_fs.c_str());
  StringC bss(s_bs.c_str());

  StringTokenizer fst(fss);
  StringTokenizer bst(bss);

  const Value* fv = colorParser(fst);
  const Value* bv = colorParser(bst);

  if (fv == NULL || bv == NULL) {
    delete fv;
    delete bv;

    std::string s_name = node.get_nodeName();
    Globals::logger(LOG_WARNING, "malformed color attribute in configuration file, `%s' element", s_name.c_str());

    return false;
  }

  f = ToRGB(fv);
  b = ToRGB(bv);

  delete fv;
  delete bv;

  return true;
}

#endif // HAVE_GMETADOM
