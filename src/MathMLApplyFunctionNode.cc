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

#include "unidefs.h"
#include "traverseAux.hh"
#include "BoundingBox.hh"
#include "MathMLElement.hh"
#include "MathMLRowElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLApplyFunctionNode.hh"
#include "MathMLEmbellishedOperatorElement.hh"

MathMLApplyFunctionNode::MathMLApplyFunctionNode() : MathMLSpaceNode(0, BREAK_NO)
{
}

MathMLApplyFunctionNode::~MathMLApplyFunctionNode()
{
}

void
MathMLApplyFunctionNode::Setup(class RenderingEnvironment* env)
{
  assert(env != NULL);  
  sppm = env->GetScaledPointsPerEm();
}

void
MathMLApplyFunctionNode::DoLayout()
{
  // the following calculation cannot be done at setup time because
  // we need to know if the element *next* to this is a fence, and this
  // information is computed at setup time. Since setup proceeds from
  // left to right, at setup time for this node the information is not
  // available

  assert(GetParent() != NULL);

  box.Set(0, 0, 0);

  if (!GetParent()->IsOperator()) return;

  MathMLElement* next = findRightSibling(GetParent());
  if (next == NULL) return;

  switch (next->IsA()) {
  case TAG_MFENCED:
    return;

  case TAG_MO:
    {
      MathMLOperatorElement* coreOp;

      if (next->IsEmbellishedOperator()) {
	MathMLEmbellishedOperatorElement* op = TO_EMBELLISHED_OPERATOR(next);
	assert(op != NULL);
	coreOp = op->GetCoreOperator();
      } else {
	coreOp = TO_OPERATOR(next);
      }
      assert(coreOp != NULL);

      if (coreOp->IsFence()) return;
    }

    // OK fallback

  default:
    {
      // FIXME: the following constant should be defined somewhere
      box.Set((sppm * 5) / 18, 0, 0);
    }
  }
}
