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
#include <assert.h>
#include <stdlib.h>

#include "MathEngine.hh"
#include "StringUnicode.hh"
#include "Configuration.hh"
#include "AttributeParser.hh"
#include "MathMLParseFile.hh"
#include "ValueConversion.hh"

Configuration::Configuration(void)
{
  colorSet = linkColorSet = selectColorSet = false;
}

Configuration::~Configuration()
{
  while (dictionaries.GetSize() > 0) {
    String* s = dictionaries.RemoveFirst();
    delete s;
  }

  while (entities.GetSize() > 0) {
    String* s = entities.RemoveFirst();
    delete s;
  }

  while (fonts.GetSize() > 0) {
    String* s = fonts.RemoveFirst();
    delete s;
  }

  while (t1Configs.GetSize() > 0) {
    String* s = t1Configs.RemoveFirst();
    delete s;
  }
}

#if defined(HAVE_MINIDOM)

bool
Configuration::Load(const char* confPath)
{
  assert(confPath != NULL);

  MathEngine::logger(LOG_DEBUG, "loading configuration from `%s'...", confPath);

  mDOMDocRef doc = MathMLParseFile(confPath, false);
  if (doc == NULL) return false;

  mDOMNodeRef root = mdom_doc_get_root_node(doc);
  if (root == NULL) {
    mdom_doc_free(doc);
    MathEngine::logger(LOG_WARNING, "configuration file `%s' has no root node", confPath);
    return false;
  }

  if (!mdom_string_eq(mdom_node_get_name(root), DOM_CONST_STRING("math-engine-configuration"))) {
    mdom_doc_free(doc);
    MathEngine::logger(LOG_WARNING, "configuration file `%s': could not find root element", confPath);
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
	MathEngine::logger(LOG_DEBUG, "found dictionary path `%s'", C_STRING(path));
	String* s = new StringC(C_STRING(path));
	s->TrimSpacesLeft();
	s->TrimSpacesRight();
	dictionaries.Append(s);
      }
    } else if (mdom_string_eq(name, DOM_CONST_STRING("font-configuration-path"))) {
      mDOMStringRef path = mdom_node_get_content(p);
      if (path != NULL) {
	MathEngine::logger(LOG_DEBUG, "found font configuration path `%s'", C_STRING(path));
	String* s = new StringC(C_STRING(path));
	s->TrimSpacesLeft();
	s->TrimSpacesRight();
	fonts.Append(s);
      }
    } else if (mdom_string_eq(name, DOM_CONST_STRING("entities-table-path"))) {
      mDOMStringRef path = mdom_node_get_content(p);
      if (path != NULL) {
	MathEngine::logger(LOG_DEBUG, "found entities table path `%s'", C_STRING(path));
	String* s = new StringC(C_STRING(path));
	s->TrimSpacesLeft();
	s->TrimSpacesRight();
	entities.Append(s);
      }
    } else if (mdom_string_eq(name, DOM_CONST_STRING("t1-config-path"))) {
      mDOMStringRef path = mdom_node_get_content(p);
      if (path != NULL && t1Configs.GetSize() == 0) {
	MathEngine::logger(LOG_DEBUG, "found t1lib config path `%s'", C_STRING(path));
	String* s = new StringC(C_STRING(path));
	s->TrimSpacesLeft();
	s->TrimSpacesRight();
	t1Configs.Append(s);
      }
    } else if (mdom_string_eq(name, DOM_CONST_STRING("font-size"))) {
      mDOMStringRef attr = mdom_node_get_attribute(p, DOM_CONST_STRING("size"));
      if (attr == NULL) {
	MathEngine::logger(LOG_WARNING, "malformed `font-size' element, cannot find `size' attribute");
      } else {
	fontSize = atoi(C_STRING(attr));
	MathEngine::logger(LOG_DEBUG, "default font size set to %d points", fontSize);
	fontSizeSet = true;
      }
    } else if (mdom_string_eq(name, DOM_CONST_STRING("color"))) {
      colorSet = ParseColor(p, foreground, background);
      if (colorSet) MathEngine::logger(LOG_DEBUG, "default color set to %06x %06x", foreground, background);
      else MathEngine::logger(LOG_WARNING, "color parsing error in configuration file");
    } else if (mdom_string_eq(name, DOM_CONST_STRING("link-color"))) {
      linkColorSet = ParseColor(p, linkForeground, linkBackground);
      if (linkColorSet) MathEngine::logger(LOG_DEBUG, "default link color set to %06x %06x", linkForeground, linkBackground);
      else MathEngine::logger(LOG_WARNING, "color parsing error in configuration file");
    } else if (mdom_string_eq(name, DOM_CONST_STRING("select-color"))) {
      selectColorSet = ParseColor(p, selectForeground, selectBackground);
      if (selectColorSet) MathEngine::logger(LOG_DEBUG, "default selection color set to %06x %06x", selectForeground, selectBackground);
      else MathEngine::logger(LOG_WARNING, "color parsing error in configuration file");
    } else if (!mdom_node_is_blank(p)) {
      MathEngine::logger(LOG_WARNING, "unrecognized entry %s in configuration file (ignored)",
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
    MathEngine::logger(LOG_WARNING, "malformed `%s' element in configuration file",
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

    MathEngine::logger(LOG_WARNING, "malformed color attribute in configuration file, `%s' element",
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

  MathEngine::logger(LOG_DEBUG, "loading configuration from `%s'...", confPath);

  try {
    GMetaDOM::Document doc = MathMLParseFile(confPath, false);

    GMetaDOM::Element root = doc.get_documentElement();
    if (root == 0) {
      MathEngine::logger(LOG_WARNING, "configuration file `%s' has no root node", confPath);
      return false;
    }

    if (root.get_nodeName() != "math-engine-configuration") {
      MathEngine::logger(LOG_WARNING, "configuration file `%s': could not find root element", confPath);
      return false;
    }

    ParseConfiguration(root);

    return true;
  } catch (GMetaDOM::DOMException) {
    return false;
  }
}

void
Configuration::ParseConfiguration(const GMetaDOM::Element& node)
{
  for (GMetaDOM::Node p = node.get_firstChild(); p != 0; p = p.get_nextSibling()) {
    if (p.get_nodeType() == GMetaDOM::Node::ELEMENT_NODE) {
      GMetaDOM::Element elem(p);
      GMetaDOM::DOMString name = elem.get_nodeName();
    
      if (name == "dictionary-path") {
	GMetaDOM::DOMString path = elementValue(elem);
	if (!path.isEmpty()) {
	  char* s_path = path.c_str();
	  MathEngine::logger(LOG_DEBUG, "found dictionary path `%s'", s_path);
	  String* s = new StringC(s_path);
	  s->TrimSpacesLeft();
	  s->TrimSpacesRight();
	  dictionaries.Append(s);
	  g_free(s_path);
	}
      } else if (name == "font-configuration-path") {
	GMetaDOM::DOMString path = elementValue(elem);
	if (!path.isEmpty()) {
	  char* s_path = path.c_str();
	  MathEngine::logger(LOG_DEBUG, "found font configuration path `%s'", s_path);
	  String* s = new StringC(s_path);
	  s->TrimSpacesLeft();
	  s->TrimSpacesRight();
	  fonts.Append(s);
	  g_free(s_path);
	}
      } else if (name == "entities-table-path") {
	GMetaDOM::DOMString path = elementValue(elem);
	if (!path.isEmpty()) {
	  char* s_path = path.c_str();
	  MathEngine::logger(LOG_DEBUG, "found entities table path `%s'", s_path);
	  String* s = new StringC(s_path);
	  s->TrimSpacesLeft();
	  s->TrimSpacesRight();
	  entities.Append(s);
	  g_free(s_path);
	}
      } else if (name == "t1-config-path") {
	GMetaDOM::DOMString path = elementValue(elem);
	if (!path.isEmpty() && t1Configs.GetSize() == 0) {
	  char* s_path = path.c_str();
	  MathEngine::logger(LOG_DEBUG, "found t1lib config path `%s'", s_path);
	  String* s = new StringC(s_path);
	  s->TrimSpacesLeft();
	  s->TrimSpacesRight();
	  t1Configs.Append(s);
	  g_free(s_path);
	}
      } else if (name == "font-size") {
	GMetaDOM::DOMString attr = elem.getAttribute("size");
	if (attr.isEmpty()) {
	  MathEngine::logger(LOG_WARNING, "malformed `font-size' element, cannot find `size' attribute");
	} else {
	  char* s_attr = attr.c_str();
	  fontSize = atoi(s_attr);
	  MathEngine::logger(LOG_DEBUG, "default font size set to %d points", fontSize);
	  fontSizeSet = true;
	  g_free(s_attr);
	}
      } else if (name == "color") {
	colorSet = ParseColor(elem, foreground, background);
	if (colorSet) MathEngine::logger(LOG_DEBUG, "default color set to %06x %06x", foreground, background);
	else MathEngine::logger(LOG_WARNING, "color parsing error in configuration file");
      } else if (name == "link-color") {
	linkColorSet = ParseColor(elem, linkForeground, linkBackground);
	if (linkColorSet) MathEngine::logger(LOG_DEBUG, "default link color set to %06x %06x", linkForeground, linkBackground);
	else MathEngine::logger(LOG_WARNING, "color parsing error in configuration file");
      } else if (name == "select-color") {
	selectColorSet = ParseColor(elem, selectForeground, selectBackground);
	if (selectColorSet) MathEngine::logger(LOG_DEBUG, "default selection color set to %06x %06x", selectForeground, selectBackground);
	else MathEngine::logger(LOG_WARNING, "color parsing error in configuration file");
      } else {
	char* s_name = name.c_str();
	MathEngine::logger(LOG_WARNING, "unrecognized element `%s' in configuration file (ignored)", s_name);
	g_free(s_name);			 
      }
    } else if (!GMetaDOM::nodeIsBlank(p)) {
      MathEngine::logger(LOG_WARNING, "unrecognized node type `%d' in configuration file (ignored)", p.get_nodeType());
    }
  }
}

bool
Configuration::ParseColor(const GMetaDOM::Element& node, RGBValue& f, RGBValue& b)
{
  GMetaDOM::DOMString fs = node.getAttribute("foreground");
  GMetaDOM::DOMString bs = node.getAttribute("background");

  if (fs.isEmpty() || bs.isEmpty()) {
    char* s_name = node.get_nodeName().c_str();
    MathEngine::logger(LOG_WARNING, "malformed `%s' element in configuration file", s_name);
    g_free(s_name);
    return false;
  }

  char* s_fs = fs.c_str();
  char* s_bs = bs.c_str();
  StringC fss(s_fs);
  StringC bss(s_bs);
  g_free(s_fs);
  g_free(s_bs);

  StringTokenizer fst(fss);
  StringTokenizer bst(bss);

  const Value* fv = colorParser(fst);
  const Value* bv = colorParser(bst);

  if (fv == NULL || bv == NULL) {
    delete fv;
    delete bv;

    char* s_name = node.get_nodeName().c_str();
    MathEngine::logger(LOG_WARNING, "malformed color attribute in configuration file, `%s' element", s_name);
    g_free(s_name);

    return false;
  }

  f = ToRGB(fv);
  b = ToRGB(bv);

  delete fv;
  delete bv;

  return true;
}

#endif // HAVE_GMETADOM
