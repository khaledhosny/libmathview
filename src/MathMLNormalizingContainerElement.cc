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

#include "Layout.hh"
#include "MathEngine.hh"
#include "MathMLRowElement.hh"
#include "MathMLDummyElement.hh"
#include "MathMLNormalizingContainerElement.hh"

#if defined(HAVE_MINIDOM)
MathMLNormalizingContainerElement::MathMLNormalizingContainerElement(mDOMNodeRef node, TagId t)
#elif defined(HAVE_GMETADOM)
MathMLNormalizingContainerElement::MathMLNormalizingContainerElement(const GMetaDOM::Element& node, TagId t)
#endif
  : MathMLContainerElement(node, t)
{
}

MathMLNormalizingContainerElement::~MathMLNormalizingContainerElement()
{
}

void
MathMLNormalizingContainerElement::Normalize()
{
  if (content.GetSize() == 0 && MathEngine::ShowEmptyElements()) {
    MathMLElement* dummy = new MathMLDummyElement();
    dummy->SetParent(this);
    content.Append(dummy);
  } else if (content.GetSize() != 1) {
    MathMLContainerElement* mrow = new MathMLRowElement(NULL);
    mrow->SetParent(this);

    while (!content.IsEmpty()) {
      MathMLElement* elem = content.RemoveFirst();
      elem->SetParent(mrow);
      mrow->content.Append(elem);
    }
    content.Append(mrow);
  }
  
  MathMLContainerElement::Normalize();
}

void
MathMLNormalizingContainerElement::DoBoxedLayout(LayoutId id, BreakId bid, scaled maxWidth)
{
  if (!HasDirtyLayout(id, maxWidth)) return;

  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);

  content.GetFirst()->DoBoxedLayout(id, bid, maxWidth);
  box = content.GetFirst()->GetBoundingBox();

  ConfirmLayout(id);

  ResetDirtyLayout(id, maxWidth);

#if 0
  printf("`%s' DoBoxedLayout (%d,%d,%d) [%d,%d]\n",
	 NameOfTagId(IsA()), id, bid, sp2ipx(maxWidth),
	 sp2ipx(box.width), sp2ipx(box.GetHeight()));
#endif  
}

void
MathMLNormalizingContainerElement::RecalcBoundingBox(LayoutId id, scaled minWidth)
{
  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);

  content.GetFirst()->RecalcBoundingBox(id, minWidth);
  box = content.GetFirst()->GetBoundingBox();

  ConfirmLayout(id);
}

void
MathMLNormalizingContainerElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;

  if (HasLayout()) layout->SetPosition(x, y);
  else {
    MathMLElement* elem = GetContent();
    assert(elem != NULL);
    elem->SetPosition(x, y);
  }
}

void
MathMLNormalizingContainerElement::Render(const DrawingArea& area)
{
  if (!HasDirtyChildren()) return;

  RenderBackground(area);

  assert(content.GetSize() == 1);
  MathMLElement* elem = content.GetFirst();
  assert(elem != NULL);

  elem->Render(area);

  ResetDirty();
}

bool
MathMLNormalizingContainerElement::IsExpanding() const
{
  MathMLElement* elem = content.GetFirst();
  assert(elem != NULL);
  return elem->IsExpanding();
}

MathMLElement*
MathMLNormalizingContainerElement::GetContent() const
{
  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);
  return content.GetFirst();
}

