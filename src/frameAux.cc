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

#include "frameAux.hh"
#include "Iterator.hh"
#include "MathMLFrame.hh"
#include "MathMLTextNode.hh"
#include "MathMLTokenElement.hh"
#include "MathMLContainerElement.hh"

const BoundingBox&
getFrameBoundingBox(MathMLFrame* frame, LayoutId id)
{
  assert(frame != NULL);

  if (id == LAYOUT_AUTO || !frame->IsElement()) return frame->GetBoundingBox();
  else {
    MathMLElement* elem = TO_ELEMENT(frame);
    assert(elem != NULL);
    if (id == LAYOUT_MIN) return elem->GetMinBoundingBox();
    else return elem->GetMaxBoundingBox();
  }
}

MathMLFrame*
getFrameLeftSibling(const MathMLFrame* frame)
{
  assert(frame != NULL);
  assert(frame->GetParent() != NULL);

  if (frame->GetParent()->IsToken()) {
    MathMLTokenElement* token = TO_TOKEN(frame->GetParent());
    assert(token != NULL);

    MathMLFrame* left = NULL;
    for (Iterator<MathMLTextNode*> p(token->GetContent()); p.More(); p.Next()) {
      if (p() == frame) return left;
      left = p();
    }
  } else if (frame->GetParent()->IsContainer()) {
    MathMLContainerElement* container = TO_CONTAINER(frame->GetParent());
    assert(container != NULL);

    MathMLElement* left = NULL;
    for (Iterator<MathMLElement*> p(container->content); p.More(); p.Next()) {
      if (p() == frame) return left;
      left = p();
    }
  }

  assert(IMPOSSIBLE);
  return NULL;
}

MathMLFrame*
getFrameRightSibling(const MathMLFrame* frame)
{
  assert(frame != NULL);
  assert(frame->GetParent() != NULL);

  if (frame->GetParent()->IsToken()) {
    MathMLTokenElement* token = TO_TOKEN(frame->GetParent());
    assert(token != NULL);

    for (Iterator<MathMLTextNode*> p(token->GetContent()); p.More(); p.Next())
      if (p() == frame) {
	p.Next();
	if (p.More()) return p();
      }
  } else if (frame->GetParent()->IsContainer()) {
    MathMLContainerElement* container = TO_CONTAINER(frame->GetParent());
    assert(container != NULL);

    for (Iterator<MathMLElement*> p(container->content); p.More(); p.Next())
      if (p() == frame) {
	p.Next();
	if (p.More()) return p();
      }
  }

  return NULL;
}
