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
#include "MathMLInvisibleTimesNode.hh"

MathMLInvisibleTimesNode::MathMLInvisibleTimesNode() : MathMLSpaceNode(0, BREAK_NO)
{
}

MathMLInvisibleTimesNode::~MathMLInvisibleTimesNode()
{
}

void
MathMLInvisibleTimesNode::Setup(class RenderingEnvironment* env)
{
  assert(env != NULL);  
  sppm = env->GetScaledPointsPerEm();
}

void
MathMLInvisibleTimesNode::DoLayout()
{
  // the following calculation cannot be done at setup time because
  // we need to know if the element *next* to this is a fence, and this
  // information is computed at setup time. Since setup proceeds from
  // left to right, at setup time for this node the information is not
  // available

  assert(GetParent() != NULL);

  box.Set(0, 0, 0);

  if (!GetParent()->IsOperator()) return;

  MathMLElement* prev = findLeftSibling(GetParent());
  MathMLElement* next = findRightSibling(GetParent());
  if (prev == NULL || next == NULL) return;

  if (prev->IsA() == TAG_MI && next->IsA() == TAG_MI) {
    MathMLTokenElement* prevToken = TO_TOKEN(prev);
    MathMLTokenElement* nextToken = TO_TOKEN(next);
    assert(prevToken != NULL && nextToken != NULL);
    
    if (prevToken->GetRawContentLength() <= 1 && nextToken->GetRawContentLength() <= 1) return;
    
    // FIXME: the following constants should be defined somewhere
    box.Set((sppm * 5) / 18, 0, 0);
  } else if (prev->IsA() == TAG_MFRAC && next->IsA() == TAG_MFRAC) {
    box.Set((sppm * 3) / 18, 0, 0);
  } else if (prev->IsA() == TAG_MFRAC || next->IsA() == TAG_MFRAC) {
    box.Set((sppm * 4) / 18, 0, 0);
  }
}
