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
#include "StringUnicode.hh"
#include "MathMLTableElement.hh"
#include "MathMLTableCellElement.hh"

MathMLTableElement::MathMLTableElement(mDOMNodeRef node) :
  MathMLContainerElement(node, TAG_MTABLE)
{
  nRows    = 0;
  nColumns = 0;
  cell     = NULL;
  column   = NULL;
  row      = NULL;
  rowLabel = NULL;

  dGC[0] = dGC[1] = NULL;
}

const AttributeSignature*
MathMLTableElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_ALIGN,           tableAlignParser,         new StringC("axis"),        NULL },
    { ATTR_ROWALIGN,        rowAlignListParser,       new StringC("baseline"),    NULL },
    { ATTR_COLUMNALIGN,     columnAlignListParser,    new StringC("center"),      NULL },
    { ATTR_GROUPALIGN,      groupAlignListListParser, new StringC("{ left }"),    NULL },
    { ATTR_ALIGNMENTSCOPE,  booleanListParser,        new StringC("true"),        NULL },
    { ATTR_COLUMNWIDTH,     columnWidthListParser,    new StringC("auto"),        NULL },
    { ATTR_ROWSPACING,      numberUnitListParser,     new StringC("1.0ex"),       NULL },
    { ATTR_COLUMNSPACING,   spaceListParser,          new StringC("0.8em"),       NULL },
    { ATTR_ROWLINES,        lineTypeListParser,       new StringC("none"),        NULL },
    { ATTR_COLUMNLINES,     lineTypeListParser,       new StringC("none"),        NULL },
    { ATTR_FRAME,           lineTypeParser,           new StringC("none"),        NULL },
    { ATTR_FRAMESPACING,    tableFrameSpacingParser,  new StringC("0.4em 0.5ex"), NULL },
    { ATTR_EQUALROWS,       booleanParser,            new StringC("true"),        NULL },
    { ATTR_EQUALCOLUMNS,    booleanParser,            new StringC("true"),        NULL },
    { ATTR_DISPLAYSTYLE,    booleanParser,            new StringC("false"),       NULL },
    { ATTR_SIDE,            tableSideParser,          new StringC("right"),       NULL },
    { ATTR_MINLABELSPACING, numberUnitParser,         new StringC("0.8em"),       NULL },
    { ATTR_WIDTH,           tableWidthParser,         new StringC("auto"),        NULL },
    { ATTR_NOTVALID,        NULL,                     NULL,                       NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLContainerElement::GetAttributeSignature(id);

  return signature;
}

MathMLTableElement::~MathMLTableElement()
{
  ReleaseAuxStructures();
}

void
MathMLTableElement::Normalize()
{
  if (content.GetSize() == 0) {
    MathMLTableRowElement* mtr = new MathMLTableRowElement(NULL);
    mtr->SetParent(this);
    content.Append(mtr);

    MathMLTableCellElement* mtd = new MathMLTableCellElement(NULL);
    mtd->SetParent(mtr);
    mtr->content.Append(mtd);
  }

  for (unsigned i = 0; i < content.GetSize(); i++) {
    MathMLElement* elem = content.RemoveFirst();
    assert(elem != NULL);

    if (elem->IsA() != TAG_MTR && elem->IsA() != TAG_MLABELEDTR) {
      MathMLTableRowElement *inferredTableRow = new MathMLTableRowElement(NULL);
      inferredTableRow->content.Append(elem);

      elem->SetParent(inferredTableRow);
      inferredTableRow->SetParent(this);
      elem = inferredTableRow;
    }
    elem->Normalize();
    content.Append(elem);
  }
}

void
MathMLTableElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;

  scaled yOffset = frameVerticalSpacing - box.ascent;

  for (unsigned i = 0; i < nRows; i++) {
    scaled xOffset = frameHorizontalSpacing;

    if (HasLabels()) {
      if (rowLabel[i].labelElement != NULL &&
	  (side == TABLE_SIDE_LEFT || side == TABLE_SIDE_LEFTOVERLAP))
	  SetLabelPosition(i, x, y + yOffset + row[i].ascent);
      
      xOffset += leftPadding;
    }

    if (row[i].mtr != NULL)
      row[i].mtr->SetPosition(x + xOffset, y + yOffset + row[i].ascent);

    for (unsigned j = 0; j < nColumns; j++) {
      TableCell* cell = GetCell(i, j);

      if (cell->mtd != NULL && !cell->spanned) {
	const BoundingBox& cellBox = cell->mtd->GetBoundingBox();
	cell->mtd->SetPosition(x + xOffset, y + yOffset + cellBox.ascent);
      }
      
      xOffset += column[j].width;
      if (j < nColumns - 1) xOffset += column[j].spacing;
    }

    if (HasLabels()) {
      xOffset += frameHorizontalSpacing;

      if (rowLabel[i].labelElement != NULL &&
	  (side == TABLE_SIDE_RIGHT || side == TABLE_SIDE_RIGHTOVERLAP))
	SetLabelPosition(i, x + xOffset, y + yOffset + row[i].ascent);
    }

    yOffset += row[i].GetHeight() + row[i].spacing;
  }
}

// SetLabelPosition:
// if side == LEFT? then x is the left edge of the whole table
// if side == RIGHT? then x is the right edge of the whole table (and thus
// must be adjust to accomodate the label)
void
MathMLTableElement::SetLabelPosition(unsigned i, scaled x, scaled y)
{
  assert(i < nRows);
  assert(rowLabel != NULL);
  assert(rowLabel[i].labelElement != NULL);

  const BoundingBox& labelBox = rowLabel[i].labelElement->GetBoundingBox();

  switch (rowLabel[i].rowAlign) {
  case ROW_ALIGN_BOTTOM:
    y += row[i].descent - labelBox.descent;
    break;
  case ROW_ALIGN_CENTER:
    y += (row[i].GetHeight() - labelBox.GetHeight()) / 2 +
      labelBox.ascent - row[i].ascent;
    break;
  case ROW_ALIGN_BASELINE:
    break;
  case ROW_ALIGN_AXIS:
    assert(IMPOSSIBLE);
    break;
  case ROW_ALIGN_TOP:
    y += labelBox.ascent - row[i].ascent;
    break;
  default:
    break;
  }

  scaled columnWidth;
  if (side == TABLE_SIDE_LEFT || side == TABLE_SIDE_LEFTOVERLAP) columnWidth = leftPadding;
  else columnWidth = box.width - leftPadding - tableWidth;

  switch (rowLabel[i].columnAlign) {
  case COLUMN_ALIGN_RIGHT:
    x += columnWidth - labelBox.width;
    break;
  case COLUMN_ALIGN_CENTER:
    x += (columnWidth - labelBox.width) / 2;
    break;
  case COLUMN_ALIGN_LEFT:
  default:
    break;
  }

  rowLabel[i].labelElement->SetPosition(x, y);
}

void
MathMLTableElement::RenderTableBackground(const DrawingArea& area)
{
  if (bGC[IsSelected()] == NULL) {
    GraphicsContextValues values;
    values.foreground = values.background = IsSelected() ? area.GetSelectionBackground() : background;
    bGC[IsSelected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_BACKGROUND);
  }

  if (HasDirtyBackground()) {
    area.Clear(bGC[IsSelected()], GetX(), GetY(), box);
  }

#if 0   
  for (unsigned i = 0; i < nRows; i++) {
    assert(row[i].mtr != NULL);
    if (row[i].mtr->IsSelected()) {
      row[i].mtr->RenderBackground(area);
      return;
    }
  }
#endif
}

void
MathMLTableElement::Render(const DrawingArea& area)
{
  if (!HasDirtyChildren()) return;

  MathMLContainerElement::Render(area);

  if (fGC[IsSelected()] == NULL) {
    GraphicsContextValues values;
    values.foreground = IsSelected() ? area.GetSelectionForeground() : color;
    values.lineStyle = LINE_STYLE_SOLID;
    values.lineWidth = lineThickness;
    fGC[IsSelected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_LINE_STYLE);

    values.lineStyle = LINE_STYLE_DASHED;
    dGC[IsSelected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_LINE_STYLE);
  }

  //area.DrawRectangle(gc, GetX(), GetY(), box);

  if (frame != TABLE_LINE_NONE) {
    Rectangle rect;

    if (HasLabels()) {
      rect.x = GetX() + leftPadding;
      rect.y = GetY() - box.ascent;    
      rect.width = tableWidth;
      rect.height = box.GetHeight();
    } else
      box.ToRectangle(GetX(), GetY(), rect);

    area.DrawRectangle((frame == TABLE_LINE_DASHED) ? dGC[IsSelected()] : fGC[IsSelected()], rect);
  }

  scaled yOffset = frameVerticalSpacing - box.ascent;
  for (unsigned i = 0; i < nRows; i++) {
    scaled xOffset = frameHorizontalSpacing;
    if (HasLabels()) xOffset += leftPadding;

    for (unsigned j = 0; j < nColumns; j++) {
      TableCell* cell = GetCell(i, j);

      if (i != nRows - 1 && row[i].lineType != TABLE_LINE_NONE) {
	// horizontal lines
	if (cell->rowSpan <= 1) {
	  scaled lineX = position.x + xOffset;
	  scaled lineY = position.y + yOffset + row[i].GetHeight() + row[i].spacing / 2;
	  scaled len = column[j].width;

	  if (j == 0) {
	    lineX -= frameHorizontalSpacing;
	    len   += frameHorizontalSpacing;
	  } else {
	    lineX -= column[j - 1].spacing / 2;
	    len   += column[j - 1].spacing / 2;
	  }

	  if (j == nColumns - 1) {
	    len   += frameHorizontalSpacing;
	  } else {
	    len   += column[j].spacing / 2;
	  }

	  area.DrawLine((row[i].lineType == TABLE_LINE_DASHED) ? dGC[IsSelected()] : fGC[IsSelected()],
			lineX, lineY, lineX + len, lineY);
	}
      }

      if (j != nColumns - 1 && column[j].lineType != TABLE_LINE_NONE) {
	// vertical lines
	if (cell->colSpan <= 1) {
	  scaled lineX = position.x + xOffset + column[j].width + column[j].spacing / 2;
	  scaled lineY = position.y + yOffset;
	  scaled len = row[i].GetHeight();

	  if (i == 0) {
	    lineY -= frameVerticalSpacing;
	    len   += frameVerticalSpacing;
	  } else {
	    lineY -= row[i - 1].spacing / 2;
	    len   += row[i - 1].spacing / 2;
	  }
	  if (i == nRows - 1) {
	    len   += frameVerticalSpacing;
	  } else {
	    len   += row[i].spacing / 2;
	  }

	  area.DrawLine((column[j].lineType == TABLE_LINE_DASHED) ? dGC[IsSelected()] : fGC[IsSelected()],
			lineX, lineY, lineX, lineY + len);
	}
      }

      xOffset += column[j].width + column[j].spacing;
    }

    yOffset += row[i].GetHeight() + row[i].spacing;
  }

  ResetDirty();
}

MathMLElement*
MathMLTableElement::Inside(scaled x, scaled y)
{
  if (!IsInside(x, y)) return NULL;
  
  for (unsigned i = 0; i < nRows; i++)
    for (unsigned j = 0; j < nColumns; j++)
      if (cell[i][j].mtd != NULL && !cell[i][j].spanned) {
	MathMLElement* inside = cell[i][j].mtd->Inside(x, y);
	
	if (inside != NULL) return inside;
      }

  return MathMLContainerElement::Inside(x, y);
}

bool
MathMLTableElement::IsExpanding() const
{
  return nFit > 0;
}

// SetDirty: Tables redefine this method for optimization. In fact,
// if the table has no lines it behaves just as a container, and
// only the cells covered by "rect" are effectively rendered again,
// not the whole table.
void
MathMLTableElement::SetDirty(const Rectangle* rect)
{
  bool hasLines = false;

  for (unsigned i = 0; !hasLines && i < nRows; i++)
    hasLines = row[i].lineType != TABLE_LINE_NONE;

  for (unsigned j = 0; !hasLines && j < nColumns; j++)
    hasLines = column[j].lineType != TABLE_LINE_NONE;

  dirtyBackground =
    (GetParent() != NULL && (GetParent()->IsSelected() != IsSelected())) ? 1 : 0;

  if (IsDirty()) return;
  if (rect != NULL && !shape->Overlaps(*rect)) return;

  if (hasLines) {
    dirty = 1;
    SetDirtyChildren();
  }

  for (Iterator<MathMLElement*> elem(content); elem.More(); elem.Next()) {
    assert(elem() != NULL);
    elem()->SetDirty(rect);
  }  
}

void
MathMLTableElement::ReleaseGCs()
{
  MathMLContainerElement::ReleaseGCs();
  dGC[0] = dGC[1] = NULL;
}
