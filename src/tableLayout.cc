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
#include "traverseAux.hh"
#include "MathMLTableElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLTableCellElement.hh"

void
MathMLTableElement::DoBoxedLayout(LayoutId id, BreakId bid, scaled availWidth)
{
  if (!HasDirtyLayout(id, availWidth)) return;

  scaled aAvailWidth = PrepareLabelsLayout(id, availWidth);

  if (id == LAYOUT_MIN) DoHorizontalMinimumLayout();
  else DoHorizontalLayout(id, bid, aAvailWidth);

  if (HasLabels()) DoLabelsLayout(id, availWidth);

  if (id == LAYOUT_AUTO) {
    StretchyCellsLayout();
    AdjustTableWidth(availWidth);
  }

  DoVerticalLayout(id);

  box.Set(GetTableWidth(), 0, 0);
  AlignTable(GetTableHeight(), box);

  if (HasLabels()) AdjustTableLayoutWithLabels(id, availWidth);

#if 0
  cout << "`mtable' DoBoxedLayout (" << id << ',' << bid << ',' << sp2ipx(availWidth);
  cout << ") [" << sp2ipx(box.width) << ',' << sp2ipx(box.GetHeight()) << ']' << endl;
#endif

  for (unsigned i = 0; i < nRows; i++) {
    if (row[i].mtr != NULL)
      row[i].mtr->box.Set(GetColumnWidth(0, nColumns), row[i].ascent, row[i].descent);

    for (unsigned j = 0; j < nColumns; j++) {
      if (cell[i][j].mtd != NULL && !cell[i][j].spanned) {
	scaled width = GetColumnWidth(j, cell[i][j].colSpan);
	scaled height = GetRowHeight(i, cell[i][j].rowSpan);

	cell[i][j].mtd->box.Set(width, row[i].ascent, height - row[i].ascent);
      }
    }
  }

  ConfirmLayout(id);

  ResetDirtyLayout(id, availWidth);
}

void
MathMLTableElement::DoHorizontalLayout(LayoutId id, BreakId bid, scaled availWidth)
{
  // ok, the available width cannot be negative
  availWidth = scaledMax(0, availWidth);

  unsigned j;

  if (widthType == WIDTH_FIXED) availWidth = fixedWidth;
  else if (widthType == WIDTH_PERCENTAGE) availWidth = float2sp(availWidth * scaleWidth);

  // the fixed spacing is always confirmed, at this early stage
  ConfirmHorizontalFixedSpacing();

  if (equalColumns) {
    // we assume that the table is large as the whole available width
    ConfirmHorizontalScaleSpacing(availWidth);
    // we compute the available space for the table content, it is obtained
    // removing the space between columns
    scaled avail = scaledMax(0, availWidth - GetSpacingWidth());
    scaled availPerColumn = avail / nColumns;

    if (id == LAYOUT_AUTO) {
      // in any case all columns must be rendered to the max of the minimum widths!!!!!!
      for (j = 0; j < nColumns; j++)
	availPerColumn = scaledMax(availPerColumn, column[j].minimumWidth);
    }

    // then we render each column with a portion of the available space
    for (j = 0; j < nColumns; j++) ColumnLayout(j, id, bid, availPerColumn);

    if (nFit == 0 && widthType == WIDTH_AUTO) {
      // oh, the table has no fit columns, so it could be smaller than the
      // whole available space, let's find the largest column
      scaled maxC = 0;
      for (j = 0; j < nColumns; j++) maxC = scaledMax(maxC, column[j].contentWidth);

      // we assign each column the same width
      for (j = 0; j < nColumns; j++) column[j].width = maxC;

      // we compute the fixed (non-%) part of the column
      scaled fixedWidth = GetColumnWidth() + GetSpacingWidth(SPACING_FIXED);
      assert(1 - wScale > EPSILON);
      scaled tableWidth = float2sp(fixedWidth / (1 - wScale));

      ConfirmHorizontalScaleSpacing(tableWidth);
    }
  } else {
    for (j = 0; j < nColumns; j++)
      if (column[j].widthType == COLUMN_WIDTH_FIXED) {
	ColumnLayout(j, id, BREAK_GOOD, column[j].fixedWidth);
	column[j].width = scaledMax(column[j].contentWidth, column[j].fixedWidth);
      }

    scaled maxTableWidth = availWidth;
    if (nAuto + nFit == 0) {
      scaled fixedWidth = GetColumnWidth(COLUMN_WIDTH_FIXED) + GetSpacingWidth(SPACING_FIXED);
      assert(1 - wScale > EPSILON);
      maxTableWidth = float2sp(fixedWidth / (1 - wScale));
    }

    ScaleColumnsLayout(id, (nAuto + nFit == 0) ? BREAK_GOOD : bid, maxTableWidth);
    ConfirmHorizontalScaleSpacing(maxTableWidth);

#if 0
    printf("%%: %d fixed: %d fixed s: %d\n", 
	   sp2ipx(GetColumnWidth(COLUMN_WIDTH_PERCENTAGE)),
	   sp2ipx(GetColumnWidth(COLUMN_WIDTH_FIXED)),
	   sp2ipx(GetSpacingWidth(SPACING_FIXED)));
#endif

    scaled avail = maxTableWidth - GetColumnWidth(COLUMN_WIDTH_PERCENTAGE) -
      GetColumnWidth(COLUMN_WIDTH_FIXED) -
      GetSpacingWidth();
    avail = scaledMax(0, avail);

    unsigned n = nAuto + nFit;

    for (j = 0; j < nColumns; j++) {
      if (column[j].widthType == COLUMN_WIDTH_AUTO) {
	ColumnLayout(j, id, bid, avail / n);
	avail = scaledMax(0, avail - column[j].contentWidth);
	n--;
      }
    }

    if (nFit == 0 && nAuto > 0 && widthType == WIDTH_AUTO) {
      scaled tableWidth = 0;
      bool constraint = false;
      for (j = 0; j < nColumns; j++) {
	if (column[j].widthType == COLUMN_WIDTH_PERCENTAGE) {
	  assert(column[j].scaleWidth > EPSILON);
	  tableWidth = scaledMax(tableWidth, float2sp(column[j].contentWidth / column[j].scaleWidth));
	  constraint = true;
	}
      }

      scaled fixedWidth =
	GetContentWidth(COLUMN_WIDTH_AUTO) +
	GetColumnWidth(COLUMN_WIDTH_FIXED) +
	GetSpacingWidth(SPACING_FIXED);

      assert(1 - wScale > EPSILON);
      tableWidth = scaledMax(tableWidth, float2sp(fixedWidth / (1 - wScale)));

      for (j = 0; j < nColumns; j++)
	if (column[j].widthType == COLUMN_WIDTH_PERCENTAGE)
	  column[j].width = float2sp(tableWidth * column[j].scaleWidth);
      ConfirmHorizontalScaleSpacing(tableWidth);
	
      scaled delta = tableWidth -
	GetColumnWidth(COLUMN_WIDTH_PERCENTAGE) -
	GetColumnWidth(COLUMN_WIDTH_FIXED) -
	GetSpacingWidth();
      delta = scaledMax(0, delta);

      // if there are not % columns, then there is no need to reduce the
      // extra space to auto column so that their total width has a
      // give value. We set them all to the content width.
      float ratio = (constraint && avail > SP_EPSILON) ? sp2float(delta) / sp2float(avail) : 0;

      for (j = 0; j < nColumns; j++)
	if (column[j].widthType == COLUMN_WIDTH_AUTO)
	  column[j].width = column[j].contentWidth +
	    float2sp((column[j].width - column[j].contentWidth) * ratio);
    } else {
      for (j = 0; j < nColumns; j++)
	if (column[j].widthType == COLUMN_WIDTH_AUTO) column[j].width = column[j].contentWidth;

      for (j = 0; j < nColumns; j++) {
	if (column[j].widthType == COLUMN_WIDTH_FIT) {
	  ColumnLayout(j, id, bid, float2sp(sp2float(avail) / n));
	  avail = scaledMax(0, avail - scaledMax(column[j].contentWidth, column[j].width));
	  n--;
	}
      }
    }
  }

  SpannedCellsLayout(id);
}

void
MathMLTableElement::DoHorizontalMinimumLayout()
{
  // if we are doing minimum layout (which is the first layout done for
  // every table) we enforce some invariants about table constraints and
  // measures
  EnforceHorizontalInvariants();

  unsigned i;
  unsigned j;

  for (j = 0; j < nColumns; j++) {
    column[j].minimumWidth = 0;
    ColumnLayout(j, LAYOUT_MIN, BREAK_GOOD, 0);
    column[j].minimumWidth = column[j].contentWidth;
  }

  for (i = 0; i < nRows; i++) {
    for (j = 0; j < nColumns; j++) {
      unsigned n = cell[i][j].colSpan;
      if (cell[i][j].mtd != NULL && !cell[i][j].spanned && n > 1) {
	scaled minWidth = GetMinimumWidth(j, cell[i][j].colSpan);
	cell[i][j].mtd->DoBoxedLayout(LAYOUT_MIN, BREAK_GOOD, 0);
	scaled cellWidth = cell[i][j].mtd->GetMinBoundingBox().width;
	if (cellWidth > minWidth) {
	  for (unsigned k = 0; k < n; k++) 
	    if (minWidth < SP_EPSILON || equalColumns)
	      column[j + k].minimumWidth = scaledMax(column[j + k].minimumWidth, cellWidth / n);
	    else
	      column[j + k].minimumWidth =
		float2sp(sp2float(column[j + k].minimumWidth) * float2sp(cellWidth) / float2sp(minWidth));
	}
      }
    }
  }

  frameHorizontalSpacing = 0;
  for (j = 0; j + 1 < nColumns; j++)
    column[j].spacing = MIN_COLUMN_SPACING;

  for (j = 0; j < nColumns; j++)
    column[j].width = column[j].minimumWidth;
}

void
MathMLTableElement::EnforceHorizontalInvariants()
{
  for (unsigned j = 0; j < nColumns; j++)
    if (column[j].widthType == COLUMN_WIDTH_PERCENTAGE &&
	column[j].scaleWidth < EPSILON)
      column[j].widthType = COLUMN_WIDTH_AUTO;

  // let's check if there is some fit column...
  nFit = CountColumnTypes(COLUMN_WIDTH_FIT);

  if (equalColumns) {
    // ok, equalColumns has precedence over any other kind of table
    // constraint, in particular all column width are set to AUTO or
    // FIT depending of the presence of a FIT column (that's why we
    // seeked for FIT columns a few lines above)
    for (unsigned j = 0; j < nColumns; j++)
      column[j].widthType = (nFit > 0) ? COLUMN_WIDTH_FIT : COLUMN_WIDTH_AUTO;
  }

  wScale = GetHorizontalScale();

  nFit = CountColumnTypes(COLUMN_WIDTH_FIT);
  nAuto = CountColumnTypes(COLUMN_WIDTH_AUTO);
  unsigned nFixed = CountColumnTypes(COLUMN_WIDTH_FIXED);
  unsigned nScale = nColumns - nFixed - nFit - nAuto;

  if (wScale > 1) {
    NormalizeHorizontalScale(wScale);
    wScale = 1;
  }

  if (1 - wScale < EPSILON &&
      (nScale <= nColumns || CountHorizontalSpacingTypes(SPACING_FIXED) > 0)) {
    NormalizeHorizontalScale(2);
    wScale = 0.5;
  }
}

unsigned
MathMLTableElement::CountColumnTypes(ColumnWidthId id) const
{
  unsigned counter = 0;
  for (unsigned j = 0; j < nColumns; j++) if (column[j].widthType == id) counter++;
  return counter;
}

unsigned
MathMLTableElement::CountHorizontalSpacingTypes(SpacingId id) const
{
  unsigned counter = 0;
  if (frame != TABLE_LINE_NONE && frameHorizontalSpacingType == id) counter += 2;
  for (unsigned j = 0; j + 1 < nColumns; j++) if (column[j].spacingType == id) counter++;
  return counter;
}

void
MathMLTableElement::ConfirmHorizontalFixedSpacing()
{
  if (frame != TABLE_LINE_NONE) {
    if (frameHorizontalSpacingType == SPACING_FIXED)
      frameHorizontalSpacing = frameHorizontalFixedSpacing;
  } else
    frameHorizontalSpacing = 0;

  for (unsigned j = 0; j + 1 < nColumns; j++) {
    if (column[j].spacingType == SPACING_FIXED)
      column[j].spacing = column[j].fixedSpacing;
  }
}

void
MathMLTableElement::ConfirmHorizontalScaleSpacing(scaled tableWidth)
{
  assert(scaledGeq(tableWidth, 0));

  if (frame != TABLE_LINE_NONE) {
    if (frameHorizontalSpacingType == SPACING_PERCENTAGE)
      frameHorizontalSpacing = float2sp(tableWidth * frameHorizontalScaleSpacing);
  }

  for (unsigned j = 0; j + 1 < nColumns; j++) {
    if (column[j].spacingType == SPACING_PERCENTAGE)
      column[j].spacing = float2sp(tableWidth * column[j].scaleSpacing);
  }
}

float
MathMLTableElement::GetHorizontalScale() const
{
  float scale = 0;
  unsigned j;

  if (frame != TABLE_LINE_NONE)
    if (frameHorizontalSpacingType == SPACING_PERCENTAGE)
      scale += 2 * frameHorizontalScaleSpacing;

  for (j = 0; j < nColumns; j++)
    if (column[j].widthType == COLUMN_WIDTH_PERCENTAGE)
      scale += column[j].scaleWidth;

  for (j = 0; j + 1 < nColumns; j++)
    if (column[j].spacingType == SPACING_PERCENTAGE)
      scale += column[j].scaleSpacing;

  return scale;
}

float
MathMLTableElement::GetVerticalScale() const
{
  float scale = 0;

  if (frame != TABLE_LINE_NONE)
    if (frameVerticalSpacingType == SPACING_PERCENTAGE)
      scale += 2 * frameVerticalScaleSpacing;

  for (unsigned i = 0; i + 1 < nRows; i++)
    if (row[i].spacingType == SPACING_PERCENTAGE)
      scale += row[i].scaleSpacing;

  return scale;
}

scaled
MathMLTableElement::GetSpacingWidth() const
{
  scaled width = 0;

  width += 2 * frameHorizontalSpacing;

  for (unsigned j = 0; j + 1 < nColumns; j++)
    width += column[j].spacing;

  return width;
}

scaled
MathMLTableElement::GetSpacingWidth(SpacingId id) const
{
  scaled width = 0;

  if (frameHorizontalSpacingType == id)
    width += 2 * frameHorizontalSpacing;

  for (unsigned j = 0; j + 1 < nColumns; j++)
    if (column[j].spacingType == id)
      width += column[j].spacing;

  return width;
}

scaled
MathMLTableElement::GetMinimumWidth(unsigned j, unsigned n) const
{
  scaled width = 0;

  for (unsigned k = 0; k < n; k++)
    width += column[j + k].minimumWidth;

  return width;
}

scaled
MathMLTableElement::GetColumnWidth() const
{
  scaled width = 0;

  for (unsigned j = 0; j < nColumns; j++)
    width += column[j].width;

  return width;
}

scaled
MathMLTableElement::GetColumnWidth(unsigned j, unsigned n) const
{
  scaled width = 0;

  for (unsigned k = 0; k < n; k++) {
    width += column[j].width;
    if (k < n - 1) width += column[j].spacing;
  }

  return width;
}

scaled
MathMLTableElement::GetColumnWidth(ColumnWidthId id) const
{
  scaled width = 0;

  for (unsigned j = 0; j < nColumns; j++)
    if (column[j].widthType == id)
      width += column[j].width;

  return width;
}

scaled
MathMLTableElement::GetContentWidth() const
{
  scaled width = 0;

  for (unsigned j = 0; j < nColumns; j++)
    width += column[j].contentWidth;

  return width;
}

scaled
MathMLTableElement::GetContentWidth(ColumnWidthId id) const
{
  scaled width = 0;

  for (unsigned j = 0; j < nColumns; j++)
    if (column[j].widthType == id)
      width += column[j].contentWidth;

  return width;
}

void
MathMLTableElement::ColumnLayout(unsigned j, LayoutId id, BreakId bid, scaled width)
{
  scaled columnWidth = 0;

  for (unsigned i = 0; i < nRows; i++) {
    TableCell& tableCell = cell[i][j];
    if (tableCell.mtd != NULL &&
	!tableCell.spanned && tableCell.colSpan == 1) {
      bool stretchyCell = isStretchyOperator(tableCell.mtd->content.GetFirst());

      if (id != LAYOUT_AUTO || !stretchyCell) {
	// CONFLICT: well, hard to explain. It is obvious that group
	// alignment must be taken into account here, because we want to
	// know the width of the column for subsequent computations, but
	// as we see stretchy cells are delayed. What if an alignment
	// group is inside a stretchy cell? This is only possible inside
	// an mrow element with one or more space-like elements (among
	// which the alignment group(s)) and the stretchy operator. However,
	// such an alignment group makes little sense, because if the
	// operator is horizontal then it should stretch to cover all the
	// available width, so alignment is useless. If the operator is
	// vertical, then it is usually inside a vertical spanned cell, and
	// again it is hard to imagine a situation where alignment is useful
	// for such a cell. For these reasons, we DO NOT consider group
	// alignment within stretchy cells.

	// first of all we reset all the alignment group so that they have
	// 0 width for the subsequent layout
	for (unsigned k = 0; k < tableCell.nAlignGroup; k++) {
	  assert(tableCell.aGroup[k].group != NULL);
	  tableCell.aGroup[k].group->SetWidth(0);
	}

	if (tableCell.nAlignGroup > 0) {
	  // since this cell is subject to alignment we cannot break its content
	  tableCell.mtd->DoBoxedLayout(id, BREAK_NO, width);
	  tableCell.mtd->CalcGroupsExtent();
	} else
	  tableCell.mtd->DoBoxedLayout(id, bid, width);
      }

      const BoundingBox& cellBox =
	stretchyCell ?
	tableCell.mtd->GetMaxBoundingBox() :
	tableCell.mtd->GetBoundingBox();

      if (cellBox.width > columnWidth) columnWidth = cellBox.width;
    }
  }

  columnWidth = scaledMax(columnWidth, ColumnGroupsLayout(j, id));

  column[j].contentWidth = scaledMax(columnWidth, column[j].minimumWidth);
  column[j].width = scaledMax(column[j].contentWidth, width);
}

scaled
MathMLTableElement::ColumnGroupsLayout(unsigned j, LayoutId id)
{
  unsigned nAlignGroup = column[j].nAlignGroup;
  if (nAlignGroup == 0) return 0;

  GroupExtent* gExtent = (nAlignGroup > 0) ? new GroupExtent[nAlignGroup] : NULL;

  unsigned k;
  unsigned i;

  for (k = 0; k < nAlignGroup; k++)
    gExtent[k].left = gExtent[k].right = 0;

  for (i = 0; i < nRows; i++) {
    TableCell& tableCell = cell[i][j];
    if (tableCell.mtd != NULL &&
	!tableCell.spanned && tableCell.colSpan == 1) {
      bool stretchyCell = isStretchyOperator(tableCell.mtd->content.GetFirst());

      if (!stretchyCell) {
	for (k = 0; k < tableCell.nAlignGroup; k++) {
	  gExtent[k].left = scaledMax(gExtent[k].left, tableCell.aGroup[k].extent.left);
	  gExtent[k].right = scaledMax(gExtent[k].right, tableCell.aGroup[k].extent.right);
	}
      }
    }
  }

  scaled alignedCellWidth = 0;
  for (k = 0; k < nAlignGroup; k++)
    alignedCellWidth += gExtent[k].left + gExtent[k].right;

  if (id == LAYOUT_AUTO) {
    for (i = 0; i < nRows; i++) {
      TableCell& tableCell = cell[i][j];
      if (tableCell.mtd != NULL && !tableCell.spanned && tableCell.colSpan == 1) {
	bool stretchyCell = isStretchyOperator(tableCell.mtd->content.GetFirst());
	
	if (!stretchyCell) {
	  for (k = 0; k < tableCell.nAlignGroup; k++) {
	    assert(tableCell.aGroup[k].group != NULL);

	    scaled rightPrev = 0;
	    if (k > 0) rightPrev = gExtent[k - 1].right - tableCell.aGroup[k - 1].extent.right;

	    tableCell.aGroup[k].group->SetWidth(rightPrev + gExtent[k].left - tableCell.aGroup[k].extent.left);
	    tableCell.aGroup[k].group->DoBoxedLayout(LAYOUT_AUTO, BREAK_NO, 0);
	  }
	}

	// ok, a little explanation: it is not necessary that every cell has exactly the same
	// number of alignment groups, furthermore it is almost impossibile that even
	// two cells with the same number of alignment groups have exaclty the same
	// width. Alignment groups acts in the innser part of the cell, but the rightmost
	// group has no right padding. However, we must be sure for every cell with alignment
	// groups to have the very same width, because of alignment purposes. If such cells are to be
	// aligned within their column, then they must have the same width to preserve
	// alignment inside groups.
	tableCell.mtd->RecalcBoundingBox(id, alignedCellWidth);
      }
    }
  }
  
  delete [] gExtent;

  return alignedCellWidth;
}

void
MathMLTableElement::ScaleColumnsLayout(LayoutId id, BreakId bid, scaled tableWidth)
{
  for (unsigned j = 0; j < nColumns; j++)
    if (column[j].widthType == COLUMN_WIDTH_PERCENTAGE) {
      ColumnLayout(j, id, bid, float2sp(tableWidth * column[j].scaleWidth));
    }
}

void
MathMLTableElement::SpannedCellsLayout(LayoutId id)
{
  for (unsigned i = 0; i < nRows; i++) {
    for (unsigned j = 0; j < nColumns; j++) {
      if (cell[i][j].mtd != NULL &&
	  !cell[i][j].spanned && cell[i][j].colSpan > 1) {
	if (id == LAYOUT_MIN) {
	  cell[i][j].mtd->DoBoxedLayout(LAYOUT_MIN, BREAK_GOOD, 0);
	  const BoundingBox& cellBox = cell[i][j].mtd->GetMinBoundingBox();
	  scaled widthPerColumn = scaledMax(0, cellBox.width / cell[i][j].colSpan);
	  for (unsigned k = 0; k < cell[i][j].colSpan; k++)
	    column[j].minimumWidth = scaledMax(column[j].minimumWidth, widthPerColumn);
	} else {
	  scaled spannedWidth = GetColumnWidth(j, cell[i][j].colSpan);
	  cell[i][j].mtd->DoBoxedLayout(id, BREAK_GOOD, spannedWidth);
	}
      }
    }
  }
}

void
MathMLTableElement::StretchyCellsLayout()
{
  for (unsigned i = 0; i < nRows; i++) {
    for (unsigned j = 0; j < nColumns; j++) {
      if (cell[i][j].mtd != NULL && !cell[i][j].spanned) {
	MathMLOperatorElement* op = findStretchyOperator(cell[i][j].mtd->content.GetFirst());
	if (op != NULL) {
	  scaled width = GetColumnWidth(j, cell[i][j].colSpan);

	  if (op->GetStretch() == STRETCH_VERTICAL) {
	    scaled height = GetRowHeight(i, cell[i][j].rowSpan);
	    op->VerticalStretchTo(row[i].ascent, height - row[i].ascent);
	  } else {
	    op->HorizontalStretchTo(width);
	  }

	  cell[i][j].mtd->DoBoxedLayout(LAYOUT_AUTO, BREAK_NO, width);
	}
      }
    }
  }
}

void
MathMLTableElement::DoVerticalLayout(LayoutId id)
{
  if (id == LAYOUT_MIN) EnforceVerticalInvariants();

  ConfirmVerticalFixedSpacing();

  unsigned i;
  unsigned j;

  for (i = 0; i < nRows; i++) {
    scaled ascent = 0;
    scaled descent = 0;

    for (j = 0; j < nColumns; j++) 
      if (cell[i][j].mtd != NULL &&
	  !cell[i][j].spanned &&
	  cell[i][j].rowAlign == ROW_ALIGN_BASELINE) {
	const BoundingBox& box = cell[i][j].mtd->GetBoundingBox();
	ascent = scaledMax(ascent, box.tAscent);
	if (cell[i][j].rowSpan == 1) descent = scaledMax(descent, box.tDescent);
      }

    if (HasLabels()) {
      if (rowLabel[i].labelElement != NULL &&
	  rowLabel[i].rowAlign == ROW_ALIGN_BASELINE) {
	const BoundingBox& labelBox = rowLabel[i].labelElement->GetBoundingBox();
	ascent = scaledMax(ascent, labelBox.tAscent);
	descent = scaledMax(descent, labelBox.tDescent);
      }
    }

    for (j = 0; j < nColumns; j++)
      if (cell[i][j].mtd != NULL &&
	  !cell[i][j].spanned && cell[i][j].rowSpan == 1 &&
	  cell[i][j].rowAlign != ROW_ALIGN_BASELINE) {
	const BoundingBox& box = cell[i][j].mtd->GetBoundingBox();
	if (box.GetHeight() > ascent + descent) descent = box.GetTotalHeight() - ascent;
      }

    if (HasLabels()) {
      if (rowLabel[i].labelElement != NULL &&
	  rowLabel[i].rowAlign != ROW_ALIGN_BASELINE) {
	const BoundingBox& labelBox = rowLabel[i].labelElement->GetBoundingBox();
	if (labelBox.GetHeight() > ascent + descent) descent = labelBox.GetTotalHeight() - ascent;
      }
    }

    row[i].ascent = ascent;
    row[i].descent = descent;
  }

  SpanRowHeight(id);

  if (equalRows) {
    scaled maxHeight = 0;
    for (i = 0; i < nRows; i++)
      maxHeight = scaledMax(maxHeight, row[i].GetHeight());

    for (i = 0; i < nRows; i++)
      if (row[i].GetHeight() < maxHeight)
	row[i].descent += maxHeight - row[i].GetHeight();
  }

  scaled fixedHeight = GetRowHeight() + GetSpacingHeight(SPACING_FIXED);
  scaled tableHeight = float2sp(fixedHeight / (1 - hScale));

  ConfirmVerticalScaleSpacing(tableHeight);
}

void
MathMLTableElement::EnforceVerticalInvariants()
{
  hScale = GetVerticalScale();

  if (hScale > 1) {
    NormalizeVerticalScale(hScale);
    hScale = 1;
  }
}

void
MathMLTableElement::NormalizeHorizontalScale(float wScale)
{
  assert(wScale > EPSILON);
  unsigned j;

  if (frameHorizontalSpacingType == SPACING_PERCENTAGE)
    frameHorizontalScaleSpacing /= wScale;
  
  for (j = 0; j < nColumns; j++) {
    if (column[j].widthType == COLUMN_WIDTH_PERCENTAGE)
      column[j].scaleWidth /= wScale;
  }

  for (j = 0; j < nColumns - 1; j++) {
    if (column[j].spacingType == SPACING_PERCENTAGE)
      column[j].scaleSpacing /= wScale;
  }
}

void
MathMLTableElement::NormalizeVerticalScale(float hScale)
{
  assert(hScale > EPSILON);

  if (frameVerticalSpacingType == SPACING_PERCENTAGE)
    frameVerticalScaleSpacing /= hScale;
  
  for (unsigned i = 0; i < nRows - 1; i++) {
    if (row[i].spacingType == SPACING_PERCENTAGE)
      row[i].scaleSpacing /= hScale;
  }
}

void
MathMLTableElement::ConfirmVerticalFixedSpacing()
{
  if (frame != TABLE_LINE_NONE) {
    if (frameVerticalSpacingType == SPACING_FIXED)
      frameVerticalSpacing = frameVerticalFixedSpacing;
  } else
    frameVerticalSpacing = 0;

  for (unsigned i = 0; i + 1 < nRows; i++) {
    if (row[i].spacingType == SPACING_FIXED)
      row[i].spacing = row[i].fixedSpacing;
  }
}

void
MathMLTableElement::ConfirmVerticalScaleSpacing(scaled tableHeight)
{
  if (frame != TABLE_LINE_NONE) {
    if (frameVerticalSpacingType == SPACING_PERCENTAGE)
      frameVerticalSpacing = float2sp(tableHeight * frameVerticalScaleSpacing);
  }

  for (unsigned i = 0; i + 1 < nRows; i++) {
    if (row[i].spacingType == SPACING_PERCENTAGE)
      row[i].spacing = float2sp(tableHeight * row[i].scaleSpacing);
  }
}

void
MathMLTableElement::SpanRowHeight(LayoutId id)
{
  for (unsigned j = 0; j < nColumns; j++) {
    unsigned i = 0;
    while (i < nRows) {
      if (cell[i][j].mtd != NULL &&
	  !cell[i][j].spanned && cell[i][j].rowSpan > 1) {
	unsigned n = cell[i][j].rowSpan;
	scaled height  = GetRowHeight(i, n);
	const BoundingBox& cellBox = cell[i][j].mtd->GetBoundingBox();

	if (height < cellBox.GetTotalHeight()) {
	  // the total height of spanned rows is still smaller than the
	  // height of the single spanning cell. We have to distribute
	  // additional space among the spanned rows
	  scaled rest = cellBox.GetTotalHeight() - height;
	  while (i < n) {
	    if (i == n - 1) {
	      // it is the last column, we assign all the remaining space
	      row[i].descent += rest;
	    } else {
	      // we assign a space proportional to the column width
	      scaled thisRowRest;
	      if (height > SP_EPSILON)
		// if all the columns have 0 width we assign an equal amount
		// of space to each spanned column
		thisRowRest = (rest * (row[i].GetHeight()));
	      else
		thisRowRest = rest / n;
	      row[i].descent += thisRowRest;
	      rest -= thisRowRest;
	    }
	    i++;
	  }
	} else
	  i += n;
      } else
	i++;
    }
  }
}

scaled
MathMLTableElement::GetRowHeight(unsigned i, unsigned n) const
{
  scaled height = 0;

  for (unsigned k = 0; k < n; k++) {
    height += row[i + k].GetHeight();
    if (k < n - 1) height += row[i + k].spacing;
  }

  return height;
}

scaled
MathMLTableElement::GetRowHeight() const
{
  scaled height = 0;

  for (unsigned i = 0; i < nRows; i++)
    height += row[i].GetHeight();

  return height;
}

scaled
MathMLTableElement::GetSpacingHeight() const
{
  scaled height = 0;

  height += 2 * frameVerticalSpacing;

  for (unsigned i = 0; i + 1 < nRows; i++)
    height += row[i].spacing;

  return height;
}

scaled
MathMLTableElement::GetSpacingHeight(SpacingId id) const
{
  scaled height = 0;

  if (frameVerticalSpacingType == id)
    height += 2 * frameVerticalSpacing;

  for (unsigned i = 0; i + 1 < nRows; i++)
    if (row[i].spacingType == id)
      height += row[i].spacing;

  return height;
}

scaled
MathMLTableElement::GetTableWidth() const
{
  return GetSpacingWidth() + GetColumnWidth();
}

scaled
MathMLTableElement::GetTableHeight() const
{
  return GetSpacingHeight() + GetRowHeight();
}

void
MathMLTableElement::AdjustTableWidth(scaled availWidth)
{
  unsigned j;
  scaled tableWidth = GetTableWidth();

  if (scaledLeq(tableWidth, availWidth)) return;

  scaled extraSpace = scaledMax(0, tableWidth - GetContentWidth());
  if (extraSpace < SP_EPSILON) return;

  scaled toKill = scaledMax(0, scaledMin(tableWidth - availWidth, extraSpace));

  float ratio = 1 - sp2float(toKill) / sp2float(extraSpace);

  for (j = 0; j < nColumns; j++)
    column[j].width = column[j].contentWidth + float2sp((column[j].width - column[j].contentWidth) * ratio);

  frameHorizontalSpacing = scaledMax(MIN_COLUMN_SPACING, float2sp(frameHorizontalSpacing * ratio));

  for (j = 0; j + 1 < nColumns; j++)
    if (column[j].spacing > MIN_COLUMN_SPACING)
      column[j].spacing = scaledMax(MIN_COLUMN_SPACING, float2sp(column[j].spacing * ratio));
}

void
MathMLTableElement::AlignTable(scaled height, BoundingBox& box)
{
  if (rowNumber > static_cast<int>(nRows)) rowNumber = nRows;
  else if (-rowNumber > static_cast<int>(nRows)) rowNumber = -static_cast<int>(nRows);

  if (rowNumber < 0) rowNumber = nRows + rowNumber + 1;

  if (rowNumber == 0) {
    switch (align) {
    case TABLE_ALIGN_TOP:
      box.ascent  = 0;
      break;
    case TABLE_ALIGN_BOTTOM:
      box.ascent  = height;
      break;
    case TABLE_ALIGN_AXIS:
      box.ascent  = height / 2 + environmentAxis;
      break;
    case TABLE_ALIGN_CENTER:
    case TABLE_ALIGN_BASELINE:
    default:
      box.ascent  = height / 2;
      break;
    }
  } else {
    scaled upTo = GetRowHeight(0, rowNumber);

    upTo += frameVerticalSpacing;

    switch (align) {
    case TABLE_ALIGN_TOP:
      box.ascent = upTo - row[rowNumber - 1].GetHeight();
      break;
    case TABLE_ALIGN_BOTTOM:
      box.ascent = upTo;
      break;
    case TABLE_ALIGN_AXIS:
      box.ascent = upTo - row[rowNumber - 1].GetHeight() / 2 + environmentAxis;
      break;
    case TABLE_ALIGN_BASELINE:
      box.ascent = upTo - row[rowNumber - 1].descent;
      break;
    case TABLE_ALIGN_CENTER:
    default:
      box.ascent = upTo - row[rowNumber - 1].GetHeight() / 2;
      break;      
    }
  }

  box.descent = height - box.ascent;

  box.tAscent = box.ascent;
  box.tDescent = box.descent;
}

// PrepareLabelsLayout: this method is for deciding whether the labels
// have to be placed aside the table or if they will overlap. After
// deciding that, the method will return the effective available width
// to render the table.
scaled
MathMLTableElement::PrepareLabelsLayout(LayoutId id, scaled availWidth)
{
  // overlappingLabels = false;
  scaled aAvailWidth = availWidth;

  if (id == LAYOUT_AUTO && HasLabels()) {
    // FIXME: what if the minLabelSpacing is a percentage value?
    assert(minLabelSpacingType == SPACING_FIXED);
    minLabelSpacing = scaledMax(0, minLabelFixedSpacing);
    
    scaled maxLabelWidth = GetMaxLabelWidth();
    
#if 0
    if (scaledLeq(GetMaxBoundingBox().width - maxLabelWidth - minLabelSpacing, availWidth) &&
	availWidth < GetMaxBoundingBox().width &&
	(side == TABLE_SIDE_LEFTOVERLAP || side == TABLE_SIDE_RIGHTOVERLAP)) {
      // ok, there is not enough room to render the labels aside the
      // table, however is we drop the labels the table could fit well, so
      // the labels will overlap
      overlappingLabels = true;
    } else {
#endif
      // labels aside the table, compute the available space for the table itself
      aAvailWidth -= maxLabelWidth - minLabelSpacing;
      aAvailWidth = scaledMax(0, aAvailWidth);
#if 0
    }
#endif
  }

  return aAvailWidth;
}

scaled
MathMLTableElement::GetMaxLabelWidth() const
{  
  assert(rowLabel != NULL);

  scaled width = 0;
  for (unsigned i = 0; i < nRows; i++) {
    if (rowLabel[i].labelElement != NULL)
      width = scaledMax(width, rowLabel[i].labelElement->GetMaxBoundingBox().width);
  }

  return width;
}

void
MathMLTableElement::DoLabelsLayout(LayoutId id, scaled availWidth)
{
  assert(rowLabel != NULL);

  scaled availForLabels = scaledMax(0, availWidth - GetTableWidth() - minLabelFixedSpacing);

  for (unsigned i = 0; i < nRows; i++) {
    if (rowLabel[i].labelElement != NULL)
      rowLabel[i].labelElement->DoBoxedLayout(id, BREAK_NO, availForLabels);
  }
}

void
MathMLTableElement::AdjustTableLayoutWithLabels(LayoutId id, scaled availWidth)
{
  assert(rowLabel != NULL);

  tableWidth = box.width;
  labelsWidth = 0;
  leftPadding = 0;

  for (unsigned i = 0; i < nRows; i++) {
    if (rowLabel[i].labelElement != NULL) {
      const BoundingBox& labelBox = rowLabel[i].labelElement->GetBoundingBox();
      labelsWidth = scaledMax(labelsWidth, labelBox.width);
    }
  }  

  tableWidth = box.width;

  if (id == LAYOUT_AUTO && availWidth > labelsWidth + minLabelSpacing + tableWidth) {
    scaled extra = availWidth - tableWidth;

    if (extra > (minLabelSpacing + labelsWidth) * 2) {
      leftPadding = extra / 2;
    } else {
      if (side == TABLE_SIDE_LEFT || side == TABLE_SIDE_LEFTOVERLAP)
	leftPadding = labelsWidth + minLabelSpacing;
      else
	leftPadding = extra - minLabelSpacing - labelsWidth;
    }

    box.width = scaledMax(availWidth, tableWidth + labelsWidth + minLabelSpacing);
  } else {
    if (side == TABLE_SIDE_LEFT || side == TABLE_SIDE_LEFTOVERLAP)
      leftPadding = labelsWidth + minLabelSpacing;
    else
      leftPadding = 0;

    box.width += labelsWidth + minLabelSpacing;
  }
}
