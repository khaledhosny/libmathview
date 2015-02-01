// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#include <config.h>
#include <assert.h>
#include <algorithm>

#include "traverseAux.hh"
#include "MathMLElement.hh"
#include "MathMLRowElement.hh"
#include "MathMLActionElement.hh"
#include "MathMLOperatorElement.hh"
// the following are needed for the dynamic casts
#include "MathMLScriptElement.hh"
#include "MathMLUnderOverElement.hh"
#include "MathMLFractionElement.hh"
#include "MathMLMultiScriptsElement.hh"
#include "MathMLStyleElement.hh"
#include "MathMLPhantomElement.hh"
#include "MathMLPaddedElement.hh"

SmartPtr<MathMLElement>
findEmbellishedOperatorRoot(const SmartPtr<MathMLElement>& op)
{
  SmartPtr<MathMLElement> root = op;

  while (root && root->getParent<MathMLElement>())
    {
      SmartPtr<MathMLElement> newRoot = root->getParent<MathMLElement>();
      if (newRoot->getCoreOperator() != op) return root;
      root = newRoot;
    }

  return root;
}

#if 0

SmartPtr<MathMLElement>
findEmbellishedOperatorRoot(const SmartPtr<MathMLElement>& root)
{
  assert(root);

  if (!root->getParent()) return root;

  SmartPtr<MathMLContainerElement> rootParent = root->getParent<MathMLContainerElement>();
  assert(rootParent);

  if (is_a<MathMLRowElement>(rootParent))
    {
      SmartPtr<MathMLRowElement> row = smart_cast<MathMLRowElement>(rootParent);
      assert(row);

      for (std::vector< SmartPtr<MathMLElement> >::const_iterator i = row->GetContent().begin();
	   i != row->GetContent().end();
	   i++)
	{
	  SmartPtr<MathMLElement> elem = *i;
	  assert(elem);
	  if (!elem->IsSpaceLike() && root != elem) return root;
	}

      return findEmbellishedOperatorRoot(rootParent);
    }
  else if (is_a<MathMLScriptElement>(rootParent) ||
	   is_a<MathMLUnderOverElement>(rootParent) ||
	   is_a<MathMLMultiScriptsElement>(rootParent) ||
	   is_a<MathMLFractionElement>(rootParent) ||
	   is_a<MathMLSemanticsElement>(rootParent))
    {
      SmartPtr<MathMLLinearContainerElement> cont = smart_cast<MathMLLinearContainerElement>(rootParent);
      assert(cont);

      if (cont->GetSize() > 0 && cont->GetChild(0) != root) return root;
      else return findEmbellishedOperatorRoot(rootParent);
    }
  else if (is_a<MathMLStyleElement>(rootParent) ||
	   is_a<MathMLPhantomElement>(rootParent) ||
	   is_a<MathMLPaddedElement>(rootParent))
    return findEmbellishedOperatorRoot(rootParent);
  else
    return root;
}
#endif

SmartPtr<MathMLOperatorElement>
findStretchyOperator(const SmartPtr<MathMLElement>& elem)
{
  if (elem)
    if (SmartPtr<MathMLOperatorElement> coreOp = elem->getCoreOperator())
      if (coreOp->IsStretchy()) return coreOp;
  return nullptr;
}

#if 0
SmartPtr<MathMLElement>
findCommonAncestor(const SmartPtr<MathMLElement>& first, const SmartPtr<MathMLElement>& last)
{
  assert(first);
  assert(last);

  SmartPtr<MathMLElement> firstP(first);
  SmartPtr<MathMLElement> lastP(last);

  if (firstP != lastP)
    {
      unsigned firstDepth = first->getDepth();
      unsigned lastDepth  = last->getDepth();

      while (firstP && firstDepth > lastDepth)
	{
	  firstP = firstP->getParent();
	  firstDepth--;
	}

      while (lastP && lastDepth > firstDepth)
	{
	  lastP = lastP->getParent();
	  lastDepth--;
	}

      assert(firstDepth == lastDepth);

      while (firstP && lastP && firstP != lastP)
	{
	  firstP = firstP->getParent();
	  lastP = lastP->getParent();
	}
    }
  
  return firstP;
}

SmartPtr<MathMLActionElement>
findActionElement(const SmartPtr<MathMLElement>& elem)
{
  SmartPtr<MathMLElement> elemP(elem);

  while (elemP && !is_a<MathMLActionElement>(elemP))
    elemP = elemP->getParent();

  return (elemP) ? smart_cast<MathMLActionElement>(elemP) : SmartPtr<MathMLActionElement>(0);
}
#endif

SmartPtr<MathMLElement>
findRightmostChild(const SmartPtr<MathMLElement>& elem)
{
  if (SmartPtr<MathMLRowElement> row = smart_cast<MathMLRowElement>(elem))
    {
      if (row->getSize() == 0) return elem;
      else return findRightmostChild(row->getChild(row->getSize() - 1));
    }
  else
    return elem;
}

SmartPtr<MathMLElement>
findLeftmostChild(const SmartPtr<MathMLElement>& elem)
{
  if (SmartPtr<MathMLRowElement> row = smart_cast<MathMLRowElement>(elem))
    {
      if (row->getSize() == 0) return elem;
      else return findLeftmostChild(row->getChild(0));
    }
  else
    return elem;
}

SmartPtr<MathMLElement>
findRightSibling(const SmartPtr<MathMLElement>& elem)
{
  if (!elem)
    return nullptr;
  else if (SmartPtr<MathMLRowElement> row = elem->getParent<MathMLRowElement>())
    {
      auto p = std::find(row->getContent().begin(), row->getContent().end(), elem);
      assert(p != row->getContent().end());
      if (p + 1 != row->getContent().end()) return findLeftmostChild(*(p + 1));
      else return findRightSibling(row);
    }
  else
    return findRightSibling(elem->getParent<MathMLElement>());
}

SmartPtr<MathMLElement>
findLeftSibling(const SmartPtr<MathMLElement>& elem)
{
  if (!elem)
    return nullptr;
  else if (SmartPtr<MathMLRowElement> row = elem->getParent<MathMLRowElement>())
    {
      auto p = std::find(row->getContent().begin(), row->getContent().end(), elem);
      assert(p != row->getContent().end());
      if (p != row->getContent().begin()) return findRightmostChild(*(p - 1));
      else return findLeftSibling(row);
    }
  else
    return findLeftSibling(elem->getParent<MathMLElement>());
}
