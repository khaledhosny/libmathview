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

#include "Iterator.hh"
#include "traverseAux.hh"
#include "MathMLElement.hh"
#include "MathMLRowElement.hh"
#include "MathMLActionElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLEmbellishedOperatorElement.hh"

MathMLElement*
findEmbellishedOperatorRoot(MathMLElement* root)
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

      return findEmbellishedOperatorRoot(rootParent);
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
      else return findEmbellishedOperatorRoot(rootParent);
    }
  case TAG_MSTYLE:
  case TAG_MPHANTOM:
  case TAG_MPADDED:
    return findEmbellishedOperatorRoot(rootParent);
  default:
    return root;
  }
}

MathMLOperatorElement*
findCoreOperator(MathMLElement* root)
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
      return findCoreOperator(rootContainer->content.GetFirst());
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

      return (core != NULL) ? findCoreOperator(core) : NULL;
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
	return findCoreOperator(rootContainer->content.GetFirst());
      else
	return NULL;
    }
  default:
    return NULL;
  }
}

MathMLOperatorElement*
findStretchyOperator(MathMLElement* elem)
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
findStretchyOperator(MathMLElement* elem, StretchId id)
{
  MathMLOperatorElement* op = findStretchyOperator(elem);
  if (op == NULL) return NULL;
  if (op->GetStretch() != id) return NULL;
  return op;
}

MathMLElement*
findCommonAncestor(MathMLElement* first, MathMLElement* last)
{
  assert(first != NULL);
  assert(last != NULL);

  if (first != last) {
    unsigned firstDepth = first->GetDepth();
    unsigned lastDepth  = last->GetDepth();

    while (firstDepth > lastDepth) {
      first = first->GetParent();
      firstDepth--;
    }

    while (lastDepth > firstDepth) {
      last = last->GetParent();
      lastDepth--;
    }

    assert(firstDepth == lastDepth);

    while (first != NULL && last != NULL && first != last) {
      first = first->GetParent();
      last = last->GetParent();
    }
  }

  assert(first == last);

  return first;
}

MathMLActionElement*
findActionElement(MathMLElement* elem)
{
  while (elem != NULL && elem->IsA() != TAG_MACTION) elem = elem->GetParent();
  return (elem != NULL) ? TO_ACTION(elem) : NULL;
}

mDOMNodeRef
findDOMNode(MathMLElement* elem)
{
  while (elem != NULL && elem->GetDOMNode() == NULL) elem = elem->GetParent();
  return (elem != NULL) ? elem->GetDOMNode() : NULL;
}

MathMLElement*
getMathMLElement(mDOMNodeRef node)
{
  assert(node != NULL);
  // WARNING: the following is a very dangerous operation. It relies
  // of the assumption that the user will NEVER modify the user data field
  // in the DOM tree elements!!!
  MathMLElement* elem = (MathMLElement*) mdom_node_get_user_data(node);
  assert(elem != NULL);
  assert(elem->GetDOMNode() == node);
  return elem;
}

MathMLElement*
findMathMLElement(mDOMNodeRef node)
{
  MathMLElement* elem = getMathMLElement(node);
  assert(elem != NULL);

  while (elem->IsA() == TAG_MROW && TO_CONTAINER(elem)->content.GetSize() == 1) {
    elem = TO_CONTAINER(elem)->content.GetFirst();
    assert(elem != NULL);
  }

  return elem;
}

MathMLElement*
findRightmostChild(MathMLElement* elem)
{
  if (elem == NULL || elem->IsA() != TAG_MROW) return elem;
  MathMLRowElement* row = TO_ROW(elem);
  assert(row != NULL);
  if (row->content.GetSize() == 0) return elem;
  else return findRightmostChild(row->content.GetLast());
}

MathMLElement*
findLeftmostChild(MathMLElement* elem)
{
  if (elem == NULL || elem->IsA() != TAG_MROW) return elem;
  MathMLRowElement* row = TO_ROW(elem);
  assert(row != NULL);
  if (row->content.GetSize() == 0) return elem;
  else return findLeftmostChild(row->content.GetFirst());
}

MathMLElement*
findRightSibling(MathMLElement* elem)
{
  mDOMNodeRef p = findDOMNode(elem);
  if (p == NULL) return NULL;

  for (p = mdom_node_get_next_sibling(p);
       p != NULL && mdom_node_get_user_data(p) == NULL;
       p = mdom_node_get_next_sibling(p)) ;

  if (p != NULL) return findLeftmostChild(findMathMLElement(p));
  else return findRightmostChild(findRightSibling(elem->GetParent()));
}

MathMLElement*
findLeftSibling(MathMLElement* elem)
{
  mDOMNodeRef p = findDOMNode(elem);
  if (p == NULL) return NULL;

  for (p = mdom_node_get_prev_sibling(p);
       p != NULL && mdom_node_get_user_data(p) == NULL;
       p = mdom_node_get_prev_sibling(p)) ;

  if (p != NULL) return findRightmostChild(findMathMLElement(p));
  else return findLeftmostChild(findLeftSibling(elem->GetParent()));
}
