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
#include "stringAux.hh"
#include "traverseAux.hh"
#include "BoundingBox.hh"
#include "MathMLElement.hh"
#include "MathMLRowElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLApplyFunctionNode.hh"
#include "MathMLFencedElement.hh"

MathMLApplyFunctionNode::MathMLApplyFunctionNode() : MathMLSpaceNode(0)
{
}

MathMLApplyFunctionNode::~MathMLApplyFunctionNode()
{
}

void
MathMLApplyFunctionNode::Setup(class RenderingEnvironment& env)
{
  sppm = env.GetScaledPointsPerEm();
}

void
MathMLApplyFunctionNode::DoLayout(const FormattingContext&)
{
  // the following calculation cannot be done at setup time because
  // we need to know if the element *next* to this is a fence, and this
  // information is computed at setup time. Since setup proceeds from
  // left to right, at setup time for this node the information is not
  // available

  assert(GetParent());

  box.Set(0, 0, 0);

  if (!is_a<MathMLOperatorElement>(GetParent())) return;

  Ptr<MathMLElement> next = findRightSibling(GetParent());
  if (!next) return;

  if (is_a<MathMLFencedElement>(next)) return;
  
  if (Ptr<MathMLOperatorElement> coreOp = next->GetCoreOperatorTop())
    if (coreOp->IsFence()) return;

  // FIXME: the following constant should be defined somewhere
  box.Set((sppm * 5) / 18, 0, 0);
}

unsigned
MathMLApplyFunctionNode::GetLogicalContentLength() const
{
  return 1;
}

String*
MathMLApplyFunctionNode::GetRawContent() const
{
  Char ch = U_APPLYFUNCTION;
  return allocString(&ch, 1);
}
