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

#include "Layout.hh"
#include "Iterator.hh"
#include "frameAux.hh"
#include "operatorAux.hh"
#include "MathMLElement.hh"
#include "MathMLTextNode.hh"

Layout::Layout(scaled width, BreakId id)
{
  totalWidth = width;
  breakability = id;
  bestPenalty = penalty = 0;
  best = NULL;
  SetLineSpacing(0);
}

Layout::~Layout()
{
}

void
Layout::In()
{
  penalty++;
}

void
Layout::Out()
{
  assert(penalty > 0);
  penalty--;
}

int
Layout::GetPenalty(BreakId id) const
{
  assert(id != BREAK_AUTO);

  switch (id) {
  case BREAK_NO:
    return MAX_PENALTY;
  case BREAK_BAD:
    return penalty + 1;
  case BREAK_GOOD:
    return penalty;
  default:
    return 0;
  }
}

void
Layout::Append(MathMLFrame* frame, scaled spacing, BreakId id)
{
  assert(frame != NULL);
  AppendAtom(frame, spacing, id);
}

void
Layout::Append(scaled spacing, BreakId id)
{
  if (id == BREAK_YES || id == BREAK_INDENT) SetLastBreakability(id);
  else AppendAtom(NULL, spacing, id);
}

void
Layout::AppendAtom(MathMLFrame* frame, scaled spacing, BreakId id)
{
  if (id != BREAK_YES && id != BREAK_INDENT && id > breakability) id = breakability;

  scaled width = 0;
  if (frame != NULL) {
    frame->ResetLast();
    width = getFrameBoundingBox(frame, LAYOUT_MAX).width;
  }

  Atom* atom = new Atom;
  atom->frame      = frame;
  atom->spacing    = spacing;
  atom->penalty    = GetPenalty(id);

  if (content.GetSize() == 0) NewRow();
  assert(content.GetLast() != NULL);

  content.GetLast()->content.Append(atom);

  availableWidth -= width + spacing;

  if (availableWidth < 0 && best != NULL) {
    Row *row = content.GetLast();
    Row *newRow = new Row;

    availableWidth = totalWidth - content.GetLast()->BreakUpTo(best, newRow);

    row->minLineSpacing = minLineSpacing;
    row->RemoveDiscardableSpacesRight();
    availableWidth += newRow->RemoveDiscardableSpacesLeft();

    FindBreakPoint(newRow);

    content.Append(newRow);
  }

  UpdateBreakPoint(atom);

  if (id == BREAK_YES || id == BREAK_INDENT) {
    NewRow();
    if (id == BREAK_INDENT) Append(INDENT_SPACING, BREAK_NO);
  }
}

void
Layout::UpdateBreakPoint(Atom* atom)
{
  assert(atom != NULL);

  if (atom->penalty < MAX_PENALTY && atom->penalty <= bestPenalty) {
    bestPenalty = atom->penalty;
    best = atom;
  }
}

void
Layout::FindBreakPoint(Row* row)
{
  assert(row != NULL);

  bestPenalty = MAX_PENALTY;
  best = NULL;

  for (Iterator<Atom*> atom(row->content); atom.More(); atom.Next())
    UpdateBreakPoint(atom());
}

void
Layout::NewRow()
{
  if (content.GetSize() > 0) {
    Row* lastRow = content.GetLast();
    assert(lastRow != NULL);
    if (lastRow->content.GetSize() == 0) return;
  }

  Row* row = new Row;
  content.Append(row);
  availableWidth = totalWidth;
  FindBreakPoint(row);
}

void
Layout::SetMinimumLineSpacing(scaled ls)
{
  minLineSpacing = scaledMax(minLineSpacing, ls);
}

void
Layout::SetLineSpacing(scaled ls)
{
  minLineSpacing = scaledMax(0, ls);
}

void
Layout::DoLayout(LayoutId id) const
{
  for (Iterator<Row*> row(content); row.More(); row.Next())
    row()->DoLayout(id, totalWidth);
}

void
Layout::GetBoundingBox(BoundingBox& box, LayoutId id) const
{
  box.Null();
  for (Iterator<Row*> row(content); row.More(); row.Next()) {
    assert(row() != NULL);

    BoundingBox rowBox;
    row()->GetBoundingBox(rowBox, id);

    if (!row.IsFirst()) rowBox.ascent = rowBox.tAscent;
    if (!row.IsLast()) rowBox.descent = rowBox.tDescent;

    if (box.IsNull()) 
      box = rowBox;
    else {
      box.descent += rowBox.GetHeight();
      box.tDescent += rowBox.GetTotalHeight();
      box.width = scaledMax(box.width, rowBox.width);
      box.lBearing = scaledMin(box.lBearing, rowBox.lBearing);
      box.rBearing = scaledMax(box.rBearing, rowBox.rBearing);
    }
  }
}

void
Layout::SetLastBreakability(BreakId id)
{
  assert(id != BREAK_AUTO);

  if (content.GetSize() == 0) return;

  if (id != BREAK_YES && id != BREAK_INDENT && id > breakability) id = breakability;

  Row* lastRow = content.GetLast();
  assert(lastRow != NULL);
  if (lastRow->content.GetSize() == 0) return;

  Atom* last = lastRow->content.GetLast();
  assert(last != NULL);

  if (id == BREAK_YES || id == BREAK_INDENT) {
    NewRow();
    if (id == BREAK_INDENT) Append(INDENT_SPACING, BREAK_NO);
  } else {
    last->penalty = GetPenalty(id);
    UpdateBreakPoint(last);
  }
}

void
Layout::SetPosition(scaled x, scaled y, ColumnAlignId align)
{
  scaled width = 0;
  if (align != COLUMN_ALIGN_LEFT) {
    BoundingBox layoutBox;
    GetBoundingBox(layoutBox, LAYOUT_AUTO);
    width = layoutBox.width;
  }

  for (Iterator<Row*> row(content); row.More(); row.Next()) {
    assert(row() != NULL);

    BoundingBox rowBox;
    row()->GetBoundingBox(rowBox, LAYOUT_AUTO);

    if (!row.IsFirst()) rowBox.ascent = rowBox.tAscent;
    if (!row.IsLast()) rowBox.descent = rowBox.tDescent;

    scaled offset = 0;

    switch (align) {
    case COLUMN_ALIGN_LEFT:
      offset = 0;
      break;
    case COLUMN_ALIGN_RIGHT:
      offset = scaledMax(0, width - rowBox.width);
      break;
    case COLUMN_ALIGN_CENTER:
      offset = scaledMax(0, (width - rowBox.width) / 2);
      break;
    default:
      assert(IMPOSSIBLE);
      break;
    }

    if (!row.IsFirst()) y += rowBox.ascent;      
    row()->SetPosition(x + offset, y);
    y += rowBox.descent;
  }
}

Layout::Row::Row()
{
  minLineSpacing = 0;
}

scaled
Layout::Row::GetWidth(LayoutId id) const
{
  BoundingBox box;
  GetBoundingBox(box, id);
  return box.width;
}

scaled
Layout::Row::GetMinimumSpacing() const
{
  return minLineSpacing;
}

void
Layout::Row::GetBoundingBox(BoundingBox& box, LayoutId id) const
{
  box.Null();
  for (Iterator<Atom*> atom(content); atom.More(); atom.Next()) {
    assert(atom() != NULL);
    BoundingBox atomBox;
    atom()->GetBoundingBox(atomBox, id);
    box.Append(atomBox);
  }
}

scaled
Layout::Row::RemoveDiscardableSpacesRight()
{
  scaled width = 0;

  while (content.GetSize() > 0 &&
	 content.GetLast() != NULL &&
	 content.GetLast()->IsDiscardable()) {
    Atom* atom = content.RemoveLast();
    assert(atom != NULL);
    width += atom->spacing;
    delete atom;
  }

  return width;
}

scaled
Layout::Row::RemoveDiscardableSpacesLeft()
{
  scaled width = 0;

  while (content.GetSize() > 0 &&
	 content.GetFirst() != NULL &&
	 content.GetFirst()->IsDiscardable()) {
    Atom* atom = content.RemoveFirst();
    assert(atom != NULL);
    width += atom->spacing;
    delete atom;
  }

  return width;
}

void
Layout::Row::DoLayout(LayoutId id, scaled totalWidth) const
{
  unsigned nElements = 0;
  scaled fixedWidth = 0;
  scaled elemWidth = 0;

  for (Iterator<Atom*> atom1(content); atom1.More(); atom1.Next()) {
    assert(atom1() != NULL);
    if (!atom1()->IsSpace()) {
      assert(atom1()->frame != NULL);
      if (atom1()->frame->IsText()/* || atom1()->frame->IsSpace()*/) {
	fixedWidth += getFrameBoundingBox(atom1()->frame).width;
      } else {
	assert(atom1()->frame->IsElement());
	elemWidth += getFrameBoundingBox(atom1()->frame, LAYOUT_MAX).width;
	nElements++;
      }
    }

    fixedWidth += atom1()->spacing;
  }

  bool moreSpace = fixedWidth + elemWidth < totalWidth;
  scaled availableWidth = scaledMax(0, totalWidth - fixedWidth);

  for (Iterator<Atom*> atom2(content); atom2.More(); atom2.Next()) {
    if (atom2()->IsFrame() && atom2()->frame->IsElement()/* && !atom2()->frame->IsSpace()*/) {
      MathMLElement* elem = TO_ELEMENT(atom2()->frame);
      assert(elem != NULL);

      if (id == LAYOUT_AUTO) {
	if (!isStretchyOperator(elem, STRETCH_VERTICAL)) {
	  scaled availablePerElem = moreSpace ?
	    scaledMax(availableWidth / nElements, elem->GetMaxBoundingBox().width) :
	    availableWidth / nElements;
	  elem->DoBoxedLayout(id, BREAK_GOOD, availablePerElem);
	  availableWidth -= elem->GetBoundingBox().width;
	} else
	  availableWidth -= elem->GetMaxBoundingBox().width;
      } else {
	elem->DoBoxedLayout(id, (id == LAYOUT_MAX) ? BREAK_NO : BREAK_GOOD);
	availableWidth -= elem->GetBoundingBox().width;
      }
      availableWidth = scaledMax(0, availableWidth);

      nElements--;
    }
  }

#if 0
  // the following code is used to set the `last' flag for
  // the last frame in a row of the layout
  // NOTE: this code is wrong in this position, because it sets
  // last even for the last frame in the last row!
  if (content.GetSize() > 0) {
    Iterator<Atom*> p(content);
    p.ResetLast();
    while (p.More()) {
      assert(p() != NULL);
      if (!p()->IsSpace()) break;
      p.Prev();
    }

    if (p.More()) {
      assert(p() != NULL);
      assert(p()->frame != NULL);
      p()->frame->SetLast();
    }
  }
#endif
}

void
Layout::Row::SetPosition(scaled x, scaled y)
{
  for (Iterator<Atom*> atom(content); atom.More(); atom.Next()) {
    assert(atom() != NULL);
    atom()->SetPosition(x, y);
    x += atom()->GetWidth(LAYOUT_AUTO);
  }
}

scaled
Layout::Row::BreakUpTo(Atom* atom, Row* newRow)
{
  assert(atom != NULL);
  assert(newRow != NULL);

  scaled width = 0;

  while (content.GetSize() > 0 && content.GetLast() != atom) {
    Atom* atom = content.RemoveLast();
    assert(atom != NULL);
    newRow->content.AddFirst(atom);
    width += atom->GetWidth(LAYOUT_MAX);
  }

  Iterator<Atom*> a(content);
  a.ResetLast();
  while (a.More() && a()->frame == NULL) a.Prev();
  if (a.More()) {
    a()->frame->SetLast();
  }
    
  return width;
}

scaled
Layout::Atom::GetWidth(LayoutId id) const
{
  BoundingBox box;
  GetBoundingBox(box, id);
  return box.width;
}

void
Layout::Atom::GetBoundingBox(BoundingBox& box, LayoutId id) const
{
  if (frame != NULL) {
    box = getFrameBoundingBox(frame, id);
    box.Append(spacing);
  } else
    box.Set(spacing, 0, 0);
}

void
Layout::Atom::SetPosition(scaled x, scaled y)
{
  if (frame == NULL) return;
  frame->SetPosition(x, y);
}
