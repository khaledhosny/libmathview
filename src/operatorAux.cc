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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>
#include <assert.h>
#include <stddef.h>

#include "Iterator.hh"
#include "operatorAux.hh"
#include "MathMLEmbellishedOperatorElement.hh"

MathMLElement*
getEmbellishedOperatorRoot(MathMLElement* root)
{
  assert(root != NULL);

  if (root->GetParent() == NULL) return root;

  assert(root->GetParent()->IsContainer());
  MathMLContainerElement* rootParent = TO_CONTAINER(root->GetParent());
  assert(rootParent != NULL);

  switch (rootParent->IsA()) {
  case TAG_MROW:
    {
      for (Iterator<MathMLElement*> i(rootParent->content); i.More(); i.Next()) {
	MathMLElement* elem = i();
	assert(elem != NULL);

	if (!elem->IsSpaceLike() && root != elem) return root;
      }

      return getEmbellishedOperatorRoot(rootParent);
    }
  case TAG_MSUP:
  case TAG_MSUB:
  case TAG_MSUBSUP:
  case TAG_MUNDER:
  case TAG_MOVER:
  case TAG_MUNDEROVER:
  case TAG_MMULTISCRIPTS:
  case TAG_MFRAC:
  case TAG_SEMANTICS:
    {
      if (rootParent->content.GetSize() > 0 &&
	  rootParent->content.GetFirst() != root) return root;
      else return getEmbellishedOperatorRoot(rootParent);
    }
  case TAG_MSTYLE:
  case TAG_MPHANTOM:
  case TAG_MPADDED:
    return getEmbellishedOperatorRoot(rootParent);
  default:
    return root;
  }
}

MathMLOperatorElement*
getCoreOperator(MathMLElement* root)
{
  assert(root != NULL);

  if (root->IsOperator()) return TO_OPERATOR(root);

  if (!root->IsContainer()) return NULL;

  MathMLContainerElement* rootContainer = TO_CONTAINER(root);
  assert(rootContainer != NULL);

  switch (rootContainer->IsA()) {
  case TAG_MO:
    // WARNING: this is an embellished operator! We cannot use the
    // GetCoreOperator because it returns a const ptr
    {
      assert(rootContainer->content.GetSize() > 0);
      return getCoreOperator(rootContainer->content.GetFirst());
    }
  case TAG_MROW:
    {
      MathMLElement* core = NULL;

      for (Iterator<MathMLElement*> i(rootContainer->content); i.More(); i.Next()) {
	MathMLElement* elem = i();
	assert(elem != NULL);

	if (!elem->IsSpaceLike()) {
	  if (core == NULL) core = elem;
	  else return NULL;
	}
      }

      return getCoreOperator(core);
    }
  case TAG_MSUP:
  case TAG_MSUB:
  case TAG_MSUBSUP:
  case TAG_MUNDER:
  case TAG_MOVER:
  case TAG_MUNDEROVER:
  case TAG_MMULTISCRIPTS:
  case TAG_MFRAC:
  case TAG_SEMANTICS:
  case TAG_MSTYLE:
  case TAG_MPHANTOM:
  case TAG_MPADDED:
    {
      if (rootContainer->content.GetSize() > 0 &&
	  rootContainer->content.GetFirst() != NULL)
	return getCoreOperator(rootContainer->content.GetFirst());
      else
	return NULL;
    }
  default:
    return NULL;
  }
}

MathMLOperatorElement*
getStretchyOperator(MathMLElement* elem)
{
  if (elem == NULL) return NULL;

  if (elem->IsEmbellishedOperator()) {
    MathMLEmbellishedOperatorElement* eop = TO_EMBELLISHED_OPERATOR(elem);
    assert(eop != NULL);
    MathMLOperatorElement* op = eop->GetCoreOperator();
    assert(op != NULL);

    if (op->IsStretchy()) return op;
    else return NULL;
  } else if (elem->IsOperator()) {
    MathMLOperatorElement* op = TO_OPERATOR(elem);
    if (op->IsStretchy()) return op;
    else return NULL;
  } else
    return NULL;
}

MathMLOperatorElement*
getStretchyOperator(MathMLElement* elem, StretchId id)
{
  MathMLOperatorElement* op = getStretchyOperator(elem);
  if (op == NULL) return NULL;
  if (op->GetStretch() != id) return NULL;
  return op;
}

bool
isStretchyOperator(MathMLElement* elem)
{
  return getStretchyOperator(elem) != NULL;
}

bool
isStretchyOperator(MathMLElement* elem, StretchId id)
{
  return getStretchyOperator(elem, id) != NULL;
}
