// Copyright (C) 2000-2001, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://www.cs.unibo.it/helm/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <wctype.h>
#include <gdome.h>

#include "String.hh"
#include "MathML.hh"
#include "gdomeAux.h"
#include "stringAux.hh"
#include "MathMLizer.hh"
#include "MathEngine.hh"
#include "StringUnicode.hh"
#include "MathMLParseFile.hh"

MathMLizer::MathMLizer(GdomeDocument* d)
{
  assert(d != NULL);
  doc = d;
}

MathMLizer::~MathMLizer()
{
}

MathMLDocument*
MathMLizer::operator() ()
{
  GdomeException exc;
  MathMLDocument* document = new MathMLDocument(doc);
  MathMLizeNode(gdome_doc_documentElement(doc, &exc), document);
  return document;
}

// MathMLizeNode: main parsing function to recognize a MathML element.
// node -> source node in the DOM tree
// parent -> parent (parsed) node
void
MathMLizer::MathMLizeNode(GdomeElement* node, MathMLContainerElement* parent)
{
  assert(node != NULL);
  assert(parent != NULL);

  // TODO: namespaces
  //  if (node->ns != NULL && xmlStrcmp(node->ns->prefix, XMLSTRING(MATHML_NS_PREFIX))) {
  //  if (verbose) fprintf(stderr, "WARNING: skipping element with namespace `%s'\n", node->ns->prefix);
  //  return;
  //}

  GdomeException exc;
  if (gdome_n_nodeType(GDOME_N(node), &exc) != GDOME_ELEMENT_NODE) {
    MathEngine::logger(LOG_WARNING, "skipping unrecognized node (type %d)\n", gdome_n_nodeType(GDOME_N(node), &exc));
    return;
  }

  GdomeDOMString* nodeName = gdome_n_nodeName(GDOME_N(node), &exc);
  assert(nodeName != NULL);

  TagId tag = TagIdOfName(gdome_str_c(nodeName));
  MathMLElement* elem = NULL;

  switch (tag) {
  case TAG_MATH:
    elem = new MathMLmathElement(node);
    break;
  case TAG_MI:
  case TAG_MN:
    elem = new MathMLTokenElement(node, tag);
    break;
  case TAG_MO:
    elem = new MathMLOperatorElement(node);
    break;
  case TAG_MTEXT:
    elem = new MathMLTextElement(node);
    break;
  case TAG_MSPACE:
    elem = new MathMLSpaceElement(node);
    break;
  case TAG_MS:
    elem = new MathMLStringLitElement(node);
    break;
  case TAG_MROW:
    elem = new MathMLRowElement(node);
    break;
  case TAG_MFRAC:
    elem = new MathMLFractionElement(node);
    break;
  case TAG_MSQRT:
  case TAG_MROOT:
    elem = new MathMLRadicalElement(node, tag);
    break;
  case TAG_MSTYLE:
    elem = new MathMLStyleElement(node);
    break;
  case TAG_MERROR:
    elem = new MathMLErrorElement(node);
    break;
  case TAG_MPADDED:
    elem = new MathMLPaddedElement(node);
    break;
  case TAG_MPHANTOM:
    elem = new MathMLPhantomElement(node);
    break;
  case TAG_MFENCED:
    elem = new MathMLFencedElement(node);
    break;
  case TAG_MSUB:
  case TAG_MSUP:
  case TAG_MSUBSUP:
    elem = new MathMLScriptElement(node, tag);
    break;
  case TAG_MUNDER:
  case TAG_MOVER:
  case TAG_MUNDEROVER:
    elem = new MathMLUnderOverElement(node, tag);
    break;
  case TAG_MMULTISCRIPTS:
    elem = new MathMLMultiScriptsElement(node);
    break;
  case TAG_MPRESCRIPTS:
  case TAG_NONE:
    if (parent != NULL && parent->IsA() == TAG_MMULTISCRIPTS)
      elem = new MathMLElement(node, tag);
    break;
  case TAG_MTABLE:
    elem = new MathMLTableElement(node);
    break;
  case TAG_MTR:
  case TAG_MLABELEDTR:
    if (parent != NULL && parent->IsA() == TAG_MTABLE)
      elem = new MathMLTableRowElement(node, tag);
    break;
  case TAG_MTD:
    if (parent != NULL && (parent->IsA() == TAG_MTR || parent->IsA() == TAG_MLABELEDTR || parent->IsA() == TAG_MTABLE))
      elem = new MathMLTableCellElement(node);
    break;
  case TAG_MALIGNGROUP:
    elem = new MathMLAlignGroupElement(node);
    break;
  case TAG_MALIGNMARK:
    elem = new MathMLAlignMarkElement(node);
    break;
  case TAG_MACTION:
    elem = new MathMLActionElement(node);
    break;
  case TAG_MENCLOSE:
    elem = new MathMLEncloseElement(node);
    break;
  case TAG_SEMANTICS:
    elem = new MathMLSemanticsElement(node);
    break;
#if 0
  case TAG_MCHAR:
    MathEngine::logger(LOG_WARNING, "`mchar' is not a valid MathML element (ignored)");
    break;
#endif
  case TAG_NOTVALID:
  default:
    break;
  }

  if (elem != NULL) {
    parent->Append(elem);
  } else {
    if (tag != TAG_NOTVALID) {
      MathEngine::logger(LOG_WARNING, "`%s' is not allowed inside a `%s' element (ignored)",
	  NameOfTagId(tag),
	  (parent != NULL) ? NameOfTagId(parent->IsA()) : "(nil)");
    } else {
      GdomeException exc;
      GdomeDOMString* nodeName = gdome_n_nodeName(GDOME_N(node), &exc);
      assert(nodeName != NULL);

      MathEngine::logger(LOG_WARNING, "unrecognized tag `%s' (ignored)", gdome_str_c(nodeName));

      gdome_str_unref(nodeName);
    }
  }

  if      (elem == NULL) MathMLizeContainerContent(node, parent);
  else if (elem->IsContainer()) MathMLizeContainerContent(node, TO_CONTAINER(elem));
  else if (elem->IsToken()) MathMLizeTokenContent(node, TO_TOKEN(elem));
}

void
MathMLizer::MathMLizeContainerContent(GdomeElement* node, MathMLContainerElement* parent)
{
  assert(node != NULL);
  assert(parent != NULL);

  GdomeException exc;
  for (GdomeNode* p = gdome_el_firstChild(node, &exc);
       p != NULL;
       p = gdome_n_nextSibling_unref(p)) {
    if (gdome_n_nodeType(p, &exc) == GDOME_ELEMENT_NODE)
      MathMLizeNode(GDOME_EL(p), parent);
    else if (!gdome_n_isBlank(p)) {
      GdomeDOMString* name = gdome_n_nodeName(p, &exc);
      assert(name != NULL);
      MathEngine::logger(LOG_WARNING, "invalid node `%s' inside container (ignored)", gdome_str_c(name));
      gdome_str_unref(name);
    }
  }
}

void
MathMLizer::MathMLizeTokenContent(GdomeElement* node, MathMLTokenElement* parent)
{
  assert(node != NULL);
  assert(parent != NULL);

  GdomeException exc;
  String* sContent = NULL;
  for (GdomeNode* p = gdome_el_firstChild(node, &exc);
       p != NULL;
       p = gdome_n_nextSibling_unref(p)) {
    if (gdome_n_nodeType(p, &exc) == GDOME_TEXT_NODE) {
      // ok, we have a chunk of text
      GdomeDOMString* content = gdome_n_nodeValue(p, &exc);
      assert(content != NULL);

      String* s = allocString(content);
      assert(s != NULL);
      gdome_str_unref(content);

      // white-spaces are always collapsed...
      s->CollapseSpaces();

      // ...but spaces at the at the beginning (end) are deleted only if this
      // is the very first (last) chunk in the token.
      if (gdome_n_isFirst(p)) s->TrimSpacesLeft();
      if (gdome_n_isLast(p)) s->TrimSpacesRight();

      if (sContent == NULL)
	sContent = s;
      else {
	sContent->Append(*s);
	delete s;
      }
#if 0
      // entities should be substituted
    } else if (gdome_n_nodeType(p) == GDOME_) {
      String* s = NULL;
      // first of all we try to perform the substitution, maybe this entity has been
      // defined inside the document itself
      mDOMStringRef content = mdom_node_get_content(p);
      if (content != NULL) {
	s = allocString(content);
	mdom_string_free(content);
      }

      // if the entity is unknown, maybe it's one of the predefined entities of MathML
      if (s == NULL) s = MathEngine::entitiesTable.GetEntityContent(mdom_node_get_name(p));

      if (s == NULL) s = MathEngine::entitiesTable.GetErrorEntityContent();
      assert(s != NULL);

      if (sContent == NULL)
	sContent = s;
      else {
	sContent->Append(*s);
	delete s;
      }
#endif
    } else if (gdome_n_nodeType(p, &exc) == GDOME_ELEMENT_NODE) {
      if (sContent != NULL) {
	parent->Append(sContent);
	delete sContent;
	sContent = NULL;
      }

      GdomeDOMString* nodeName = gdome_n_nodeName(p, &exc);
      assert(nodeName != NULL);

      TagId tag = TagIdOfName(gdome_str_c(nodeName));

      switch (tag) {
      case TAG_MGLYPH:
	{
	  MathMLTextNode* text = SubstituteMGlyphElement(GDOME_EL(p));
	  if (text != NULL) parent->Append(text);
	}
	break;
      case TAG_MALIGNMARK:
	{
	  MathMLTextNode* text = SubstituteAlignMarkElement(GDOME_EL(p));
	  if (text != NULL) parent->Append(text);
	}
	break;
      default:
	MathEngine::logger(LOG_WARNING, "unacceptable element `%s' inside token (ignored)",
			   gdome_str_c(nodeName));
	break;
      }

      gdome_str_unref(nodeName);
    } else if (!gdome_n_isBlank(p)) {
      GdomeDOMString* name = gdome_n_nodeName(p, &exc);
      assert(name != NULL);
      MathEngine::logger(LOG_WARNING, "invalid node `%s' inside token (ignored)", gdome_str_c(name));
      gdome_str_unref(name);
    }
  }
  
  if (sContent != NULL) {
    parent->Append(sContent);
    delete sContent;
  }
}

MathMLTextNode*
MathMLizer::SubstituteMGlyphElement(GdomeElement* node)
{
  assert(node != NULL);

  GdomeDOMString* alt        = gdome_el_getAttribute_c(node, "alt");
  GdomeDOMString* fontFamily = gdome_el_getAttribute_c(node, "fontfamily");
  GdomeDOMString* index      = gdome_el_getAttribute_c(node, "index");
  assert(alt != NULL && fontFamily != NULL && index != NULL);

  if (gdome_str_isEmpty(alt) || gdome_str_isEmpty(fontFamily) || gdome_str_isEmpty(index)) {
    MathEngine::logger(LOG_WARNING, "malformed `mglyph' element (some required attribute is missing)\n");

    gdome_str_unref(alt);
    gdome_str_unref(fontFamily);
    gdome_str_unref(index);

    return new MathMLCharNode('?');
  }

  char* endPtr;
  unsigned nch = strtoul(gdome_str_c(index), &endPtr, 10);

  if (endPtr == NULL || *endPtr != '\0') {
    MathEngine::logger(LOG_WARNING, "malformed `mglyph' element (parsing error in `index' attribute)\n");
    nch = '?';
  }

  MathMLGlyphNode* glyph = new MathMLGlyphNode(gdome_str_c(alt), gdome_str_c(fontFamily), nch);

  gdome_str_unref(alt);
  gdome_str_unref(fontFamily);
  gdome_str_unref(index);

  return glyph;
}

MathMLTextNode*
MathMLizer::SubstituteAlignMarkElement(GdomeElement* node)
{
  assert(node != NULL);

  GdomeDOMString* edge = gdome_el_getAttribute_c(node, "edge");
  assert(edge != NULL);

  MarkAlignType align = MARK_ALIGN_NOTVALID;

  if (edge != NULL) {
    if      (gdome_str_equal_c(edge, "left")) align = MARK_ALIGN_LEFT;
    else if (gdome_str_equal_c(edge, "right")) align = MARK_ALIGN_RIGHT;
    else
      MathEngine::logger(LOG_WARNING,
			 "malformed `malignmark' element, attribute `edge' has invalid value `%s' (ignored)",
			 gdome_str_c(edge));
  }

  gdome_str_unref(edge);

  return new MathMLMarkNode(align);
}
