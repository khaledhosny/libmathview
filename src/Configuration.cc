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

#include <gdome.h>

#include "gdomeAux.h"
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

bool
Configuration::Load(const char* confPath)
{
  assert(confPath != NULL);

  MathEngine::logger(LOG_DEBUG, "loading configuration from `%s'...", confPath);

  GdomeException exc;
  GdomeDocument* doc = MathMLParseFile(confPath, false);
  if (doc == NULL) return false;

  GdomeElement* root = gdome_doc_documentElement(doc, &exc);
  if (root == NULL) {
    gdome_doc_unref(doc, &exc);
    MathEngine::logger(LOG_WARNING, "configuration file `%s' has no root node", confPath);
    return false;
  }

  if (!gdome_n_name_is(GDOME_N(root), "math-engine-configuration")) {
    gdome_doc_unref(doc, &exc);
    MathEngine::logger(LOG_WARNING, "configuration file `%s': could not find root element", confPath);
    return false;
  }

  ParseConfiguration(root);

  gdome_doc_unref(doc, &exc);

  return true;
}

void
Configuration::ParseConfiguration(GdomeElement* el)
{
  assert(el != NULL);
  
  GdomeException exc;
  for (GdomeNode* p = gdome_el_firstChild(el, &exc);
       p != NULL;
       p = gdome_n_nextSibling_unref(p)) {
    if (gdome_n_name_is(p, "dictionary-path")) {
      GdomeDOMString* path = gdome_n_content(p);
      assert(path != NULL);

      if (!gdome_str_isEmpty(path)) {
	MathEngine::logger(LOG_DEBUG, "found dictionary path `%s'", gdome_str_c(path));
	String* s = new StringC(gdome_str_c(path));
	s->TrimSpacesLeft();
	s->TrimSpacesRight();
	dictionaries.Append(s);
      }

      gdome_str_unref(path);
    } else if (gdome_n_name_is(p, "font-configuration-path")) {
      GdomeDOMString* path = gdome_n_content(p);
      assert(path != NULL);

      if (!gdome_str_isEmpty(path)) {
	MathEngine::logger(LOG_DEBUG, "found font configuration path `%s'", gdome_str_c(path));
	String* s = new StringC(gdome_str_c(path));
	s->TrimSpacesLeft();
	s->TrimSpacesRight();
	fonts.Append(s);
      }

      gdome_str_unref(path);
    } else if (gdome_n_name_is(p, "entities-table-path")) {
      GdomeDOMString* path = gdome_n_content(p);
      assert(path != NULL);

      if (!gdome_str_isEmpty(path)) {
	MathEngine::logger(LOG_DEBUG, "found entities table path `%s'", gdome_str_c(path));
	String* s = new StringC(gdome_str_c(path));
	s->TrimSpacesLeft();
	s->TrimSpacesRight();
	entities.Append(s);
      }

      gdome_str_unref(path);
    } else if (gdome_n_name_is(p, "t1-config-path")) {
      GdomeDOMString* path = gdome_n_content(p);
      assert(path != NULL);

      if (!gdome_str_isEmpty(path) && t1Configs.GetSize() == 0) {
	MathEngine::logger(LOG_DEBUG, "found t1lib config path `%s'", gdome_str_c(path));
	String* s = new StringC(gdome_str_c(path));
	s->TrimSpacesLeft();
	s->TrimSpacesRight();
	t1Configs.Append(s);
      }

      gdome_str_unref(path);
    } else if (gdome_n_name_is(p, "font-size")) {
      GdomeDOMString* size = gdome_el_getAttribute_c(GDOME_EL(p), "size");

      if (gdome_str_isEmpty(size)) {
	MathEngine::logger(LOG_WARNING, "malformed `font-size' element, cannot find `size' attribute");
      } else {
	fontSize = atoi(gdome_str_c(size));
	MathEngine::logger(LOG_DEBUG, "default font size set to %d points", fontSize);
	fontSizeSet = true;
      }

      gdome_str_unref(size);
    } else if (gdome_n_name_is(p, "color")) {
      colorSet = ParseColor(GDOME_EL(p), foreground, background);
      if (colorSet) MathEngine::logger(LOG_DEBUG, "default color set to %06x %06x", foreground, background);
      else MathEngine::logger(LOG_WARNING, "color parsing error in configuration file");
    } else if (gdome_n_name_is(p, "link-color")) {
      linkColorSet = ParseColor(GDOME_EL(p), linkForeground, linkBackground);
      if (linkColorSet) MathEngine::logger(LOG_DEBUG, "default link color set to %06x %06x", linkForeground, linkBackground);
      else MathEngine::logger(LOG_WARNING, "color parsing error in configuration file");
    } else if (gdome_n_name_is(p, "select-color")) {
      selectColorSet = ParseColor(GDOME_EL(p), selectForeground, selectBackground);
      if (selectColorSet) MathEngine::logger(LOG_DEBUG, "default selection color set to %06x %06x", selectForeground, selectBackground);
      else MathEngine::logger(LOG_WARNING, "color parsing error in configuration file");
    } else if (!gdome_n_isBlank(p)) {
      GdomeException exc;
      GdomeDOMString* name = gdome_n_nodeName(p, &exc);
      assert(name != NULL);

      MathEngine::logger(LOG_WARNING, "unrecognized entry %s in configuration file (ignored)",
			 gdome_str_c(name));
      
      gdome_str_unref(name);
    }
  }
}

bool
Configuration::ParseColor(GdomeElement* el, RGBValue& f, RGBValue& b)
{
  assert(el != NULL);

  GdomeException exc;
  GdomeDOMString* fs = gdome_el_getAttribute_c(el, "foreground");
  GdomeDOMString* bs = gdome_el_getAttribute_c(el, "background");
  assert(fs != NULL && bs != NULL);

  if (gdome_str_isEmpty(fs) || gdome_str_isEmpty(bs)) {
    GdomeDOMString* name = gdome_n_nodeName(GDOME_N(el), &exc);
    assert(name != NULL);

    MathEngine::logger(LOG_WARNING, "malformed `%s' element in configuration file",
		       gdome_str_c(name));

    gdome_str_unref(name);
    gdome_str_unref(fs);
    gdome_str_unref(bs);

    return false;
  }

  StringC fss(gdome_str_c(fs));
  StringC bss(gdome_str_c(bs));
  gdome_str_unref(fs);
  gdome_str_unref(bs);

  StringTokenizer fst(fss);
  StringTokenizer bst(bss);

  const Value* fv = colorParser(fst);
  const Value* bv = colorParser(bst);

  if (fv == NULL || bv == NULL) {
    GdomeDOMString* name = gdome_n_nodeName(GDOME_N(el), &exc);
    assert(name != NULL);

    delete fv;
    delete bv;

    MathEngine::logger(LOG_WARNING, "malformed color attribute in configuration file, `%s' element",
		       gdome_str_c(name));

    gdome_str_unref(name);

    return false;
  }

  f = ToRGB(fv);
  b = ToRGB(bv);

  delete fv;
  delete bv;

  return true;
}
