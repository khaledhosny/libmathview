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
#ifdef HAVE_WCTYPE_H
#include <wctype.h>
#endif
#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif

#include "String.hh"
#include "MathML.hh"
#include "minidom.h"
#include "stringAux.hh"
#include "MathMLizer.hh"
#include "MathEngine.hh"
#include "StringUnicode.hh"
#include "MathMLParseFile.hh"

MathMLizer::MathMLizer(mDOMDocRef d)
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
  MathMLDocument* document = new MathMLDocument(doc);
  MathMLizeNode(mdom_doc_get_root_node(doc), document);
  return document;
}

// MathMLizeNode: main parsing function to recognize a MathML element.
// node -> source node in the DOM tree
// parent -> parent (parsed) node
void
MathMLizer::MathMLizeNode(mDOMNodeRef node, MathMLContainerElement* parent)
{
  assert(node != NULL);
  assert(parent != NULL);

  // TODO: namespaces
  //  if (node->ns != NULL && xmlStrcmp(node->ns->prefix, XMLSTRING(MATHML_NS_PREFIX))) {
  //  if (verbose) fprintf(stderr, "WARNING: skipping element with namespace `%s'\n", node->ns->prefix);
  //  return;
  //}

  if (!mdom_node_is_element(node)) {
    MathEngine::logger(LOG_WARNING, "skipping unrecognized node (type %d)\n", mdom_node_get_type(node));
    return;
  }

  TagId tag = TagIdOfName(C_CONST_STRING(mdom_node_get_name(node)));
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
  case TAG_MCHAR:
    MathEngine::logger(LOG_WARNING, "`mchar' is not a valid MathML element (ignored)");
    break;
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
      MathEngine::logger(LOG_WARNING, "unrecognized tag `%s' (ignored)", node->name);
    }
  }

  if      (elem == NULL) MathMLizeContainerContent(node, parent);
  else if (elem->IsContainer()) MathMLizeContainerContent(node, TO_CONTAINER(elem));
  else if (elem->IsToken()) MathMLizeTokenContent(node, TO_TOKEN(elem));
}

void
MathMLizer::MathMLizeContainerContent(mDOMNodeRef node, MathMLContainerElement* parent)
{
  assert(node != NULL);
  assert(parent != NULL);

  mDOMNodeRef p = mdom_node_get_first_child(node);
  while (p != NULL) {
    if (mdom_node_is_text(p)) {
      mDOMStringRef content = mdom_node_get_content(p);
      String* text = allocString(content);
      assert(text != NULL);
      mdom_string_free(content);

      text->CollapseSpaces();
      text->TrimSpacesLeft();
      text->TrimSpacesRight();

      if (text->GetLength() > 0) {
	MathEngine::logger(LOG_WARNING, "text inside container element `%s' (ignored)\n", NameOfTagId(parent->IsA()));
      }

      delete text;
    } else
      MathMLizeNode(p, parent);

    p = mdom_node_get_next_sibling(p);
  }
}

void
MathMLizer::MathMLizeTokenContent(mDOMNodeRef node, MathMLTokenElement* parent)
{
  assert(parent != NULL);

  String* sContent = NULL;
  mDOMNodeRef p = mdom_node_get_first_child(node);
  while (p != NULL) {
    if (mdom_node_is_text(p)) {
      // ok, we have a chunk of text
      mDOMStringRef content = mdom_node_get_content(p);
      String* s = allocString(content);
      assert(s != NULL);
      mdom_string_free(content);

      // white-spaces are always collapsed...
      s->CollapseSpaces();

      // ...but spaces at the at the beginning (end) are deleted only if this
      // is the very first (last) chunk in the token.
      if (mdom_node_is_first(p)) s->TrimSpacesLeft();
      if (mdom_node_is_last(p)) s->TrimSpacesRight();

      if (sContent == NULL)
	sContent = s;
      else {
	sContent->Append(*s);
	delete s;
      }
    } else if (mdom_node_is_entity_ref(p)) {
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

      if (s == NULL) {
	MathEngine::logger(LOG_WARNING, "cannot resolve entity reference `%s', a `?' will be used instead", mdom_node_get_name(p));
	s = MathEngine::entitiesTable.GetErrorEntityContent();
      }
      assert(s != NULL);

      if (sContent == NULL)
	sContent = s;
      else {
	sContent->Append(*s);
	delete s;
      }
    } else if (mdom_node_is_element(p)) {
      if (sContent != NULL) {
	parent->Append(sContent);
	delete sContent;
	sContent = NULL;
      }

      TagId tag = TagIdOfName(C_CONST_STRING(mdom_node_get_name(p)));

      switch (tag) {
      case TAG_MCHAR:
	{
	  MathEngine::logger(LOG_WARNING, "`mchar' is not a valid MathML element. It is recognized for backward compatibility only!");
	  String* content = SubstituteMCharElement(p);
	  if (content != NULL) parent->Append(content);
	  delete content;
	}
	break;
      case TAG_MGLYPH:
	{
	  MathMLTextNode* text = SubstituteMGlyphElement(p);
	  if (text != NULL) parent->Append(text);
	}
	break;
      case TAG_MALIGNMARK:
	{
	  MathMLTextNode* text = SubstituteAlignMarkElement(p);
	  if (text != NULL) parent->Append(text);
	}
	break;
      default:
	MathEngine::logger(LOG_WARNING, "unacceptable element `%s' inside token (ignored)\n",
			   mdom_node_get_name(node));
	break;
      }
    }

    p = mdom_node_get_next_sibling(p);
  }
  
  if (sContent != NULL) {
    parent->Append(sContent);
    delete sContent;
  }
}

String*
MathMLizer::SubstituteMCharElement(mDOMNodeRef node)
{
  mDOMStringRef name = mdom_node_get_attribute(node, DOM_CONST_STRING("name"));
  String* content = NULL;

  if (name == NULL) {
    MathEngine::logger(LOG_WARNING, "malformed `mchar' element (`name' attribute required)\n");
    content = allocString(DOM_CONST_STRING("?"));
  } else {
    content = MathEngine::entitiesTable.GetEntityContent(name);
    if (content == NULL) content = MathEngine::entitiesTable.GetErrorEntityContent();
  }

  assert(content != NULL);

  if (content->GetLength() == 0) {
    delete content;
    content = NULL;
  }

  mdom_string_free(name);

  return content;
}

MathMLTextNode*
MathMLizer::SubstituteMGlyphElement(mDOMNodeRef node)
{
  assert(node != NULL);

  mDOMStringRef alt        = mdom_node_get_attribute(node, DOM_CONST_STRING("alt"));
  mDOMStringRef fontFamily = mdom_node_get_attribute(node, DOM_CONST_STRING("fontfamily"));
  mDOMStringRef index      = mdom_node_get_attribute(node, DOM_CONST_STRING("index"));

  if (alt == NULL || fontFamily == NULL || index == NULL ||
      *alt == '\0' || *fontFamily == '\0' || *index == '\0') {
    MathEngine::logger(LOG_WARNING, "malformed `mglyph' element (some required attribute is missing)\n");

    mdom_string_free(alt);
    mdom_string_free(fontFamily);
    mdom_string_free(index);

    return new MathMLCharNode('?');
  }

  char* endPtr;
  unsigned nch = strtoul(C_STRING(index), &endPtr, 10);

  if (endPtr == NULL || *endPtr != '\0') {
    MathEngine::logger(LOG_WARNING, "malformed `mglyph' element (parsing error in `index' attribute)\n");
    nch = '?';
  }

  MathMLGlyphNode* glyph = new MathMLGlyphNode(C_STRING(alt), C_STRING(fontFamily), nch);

  mdom_string_free(alt);
  mdom_string_free(fontFamily);
  mdom_string_free(index);

  return glyph;
}

MathMLTextNode*
MathMLizer::SubstituteAlignMarkElement(mDOMNodeRef node)
{
  assert(node != NULL);

  mDOMStringRef edge = mdom_node_get_attribute(node, DOM_CONST_STRING("edge"));

  MarkAlignType align = MARK_ALIGN_NOTVALID;

  if (edge != NULL) {
    if      (mdom_string_eq(edge, DOM_CONST_STRING("left"))) align = MARK_ALIGN_LEFT;
    else if (mdom_string_eq(edge, DOM_CONST_STRING("right"))) align = MARK_ALIGN_RIGHT;
    else
      MathEngine::logger(LOG_WARNING,
			 "malformed `malignmark' element, attribute `edge' has invalid value `%s' (ignored)",
			 edge);
  }
  mdom_string_free(edge);

  return new MathMLMarkNode(align);
}
