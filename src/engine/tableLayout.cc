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
#include <stddef.h>

#include "RGBColor.hh"
#include "traverseAux.hh"
#include "MathMLTableElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLTableCellElement.hh"

#include "BoundingBoxAux.hh"

#if 0
void
MathMLTableElement::DoLayout(const FormattingContext& ctxt)
{
  if (dirtyLayout(ctxt))
    {
      //std::cout << "redoing table layout with type " << this << " " << ctxt.GetLayoutType() << std::endl;
      scaled aAvailWidth = PrepareLabelsLayout(ctxt);

      if (ctxt.GetLayoutType() == FormattingContext::LAYOUT_MIN) DoHorizontalMinimumLayout();
      else DoHorizontalLayout(ctxt);

      if (HasLabels()) DoLabelsLayout(ctxt);

      if (ctxt.GetLayoutType() == FormattingContext::LAYOUT_AUTO)
	{
	  StretchyCellsLayout();
	  //AdjustTableWidth(ctxt.GetAvailableWidth());
	}

      DoVerticalLayout(ctxt.GetLayoutType());

      box.set(GetTableWidth(), 0, 0);
      AlignTable(GetTableHeight(), box);

      if (HasLabels()) AdjustTableLayoutWithLabels(ctxt);

      for (unsigned i = 0; i < nRows; i++) {
	if (row[i].mtr)
	  row[i].mtr->box.set(GetColumnWidth(0, nColumns), row[i].height, row[i].depth);

	for (unsigned j = 0; j < nColumns; j++) {
	  if (cell[i][j].mtd && !cell[i][j].spanned) {
	    scaled width = GetColumnWidth(j, cell[i][j].colSpan);
	    scaled height = GetRowHeight(i, cell[i][j].rowSpan);

	    cell[i][j].mtd->box.set(width, row[i].height, height - row[i].height);
	  }
	}
      }

      // because the table invokes directly the layout operations on its
      // cells, we have to clean the dirty-layout flag on the rows here
      // otherwise this will stop the propagation mechanism of the dirty
      // layout flag when one of the rows changes
      for (std::vector< SmartPtr<MathMLElement> >::iterator p = content.begin();
	   p != content.end();
	   p++)
	(*p)->resetDirtyLayout(ctxt);

      resetDirtyLayout(ctxt);
    }
}

void
MathMLTableElement::DoHorizontalLayout(const FormattingContext& ctxt)
{
  // ok, the available width cannot be negative
  scaled availWidth = std::max(scaled(0), ctxt.GetAvailableWidth());

  unsigned j;

  if (widthType == WIDTH_FIXED) availWidth = fixedWidth;
  else if (widthType == WIDTH_PERCENTAGE) availWidth = availWidth * scaleWidth;

  // the fixed spacing is always confirmed, at this early stage
  ConfirmHorizontalFixedSpacing();

  if (equalColumns) {
    // we assume that the table is large as the whole available width
    ConfirmHorizontalScaleSpacing(availWidth);
    // we compute the available space for the table content, it is obtained
    // removing the space between columns
    scaled avail = std::max(scaled(0), availWidth - GetSpacingWidth());
    scaled availPerColumn = avail / static_cast<int>(nColumns);

    if (ctxt.GetLayoutType() == FormattingContext::LAYOUT_AUTO) {
      // in any case all columns must be rendered to the max of the minimum widths!!!!!!
      for (j = 0; j < nColumns; j++)
	availPerColumn = std::max(availPerColumn, column[j].minimumWidth);
    }

    // then we render each column with a portion of the available space
    for (j = 0; j < nColumns; j++) ColumnLayout(j, FormattingContext(ctxt.GetLayoutType(), availPerColumn));

    if (nFit == 0 && widthType == WIDTH_AUTO) {
      // oh, the table has no fit columns, so it could be smaller than the
      // whole available space, let's find the largest column
      scaled maxC = 0;
      for (j = 0; j < nColumns; j++) maxC = std::max(maxC, column[j].contentWidth);

      // we assign each column the same width
      for (j = 0; j < nColumns; j++) column[j].width = maxC;

      // we compute the fixed (non-%) part of the column
      scaled fixedWidth = GetColumnWidth() + GetSpacingWidth(SPACING_FIXED);
      assert(1 - wScale > EPSILON);
      scaled tableWidth = fixedWidth / (1 - wScale);

      ConfirmHorizontalScaleSpacing(tableWidth);
    }
  } else {
    for (j = 0; j < nColumns; j++)
      if (column[j].widthType == COLUMN_WIDTH_FIXED) {
	ColumnLayout(j, FormattingContext(ctxt.GetLayoutType(), column[j].fixedWidth));
	column[j].width = std::max(column[j].contentWidth, column[j].fixedWidth);
      }

    scaled maxTableWidth = availWidth;
    if (nAuto + nFit == 0) {
      scaled fixedWidth = GetColumnWidth(COLUMN_WIDTH_FIXED) + GetSpacingWidth(SPACING_FIXED);
      assert(1 - wScale > EPSILON);
      maxTableWidth = fixedWidth / (1 - wScale);
    }

    ScaleColumnsLayout(FormattingContext(ctxt.GetLayoutType(), maxTableWidth));
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
    avail = std::max(scaled(0), avail);

    unsigned n = nAuto + nFit;

    for (j = 0; j < nColumns; j++) {
      if (column[j].widthType == COLUMN_WIDTH_AUTO) {
	ColumnLayout(j, FormattingContext(ctxt.GetLayoutType(), avail / static_cast<int>(n)));
	avail = std::max(scaled(0), avail - column[j].contentWidth);
	n--;
      }
    }

    if (nFit == 0 && nAuto > 0 && widthType == WIDTH_AUTO) {
      scaled tableWidth = 0;
      bool constraint = false;
      for (j = 0; j < nColumns; j++) {
	if (column[j].widthType == COLUMN_WIDTH_PERCENTAGE) {
	  assert(column[j].scaleWidth > EPSILON);
	  tableWidth = std::max(tableWidth, column[j].contentWidth / column[j].scaleWidth);
	  constraint = true;
	}
      }

      scaled fixedWidth =
	GetContentWidth(COLUMN_WIDTH_AUTO) +
	GetColumnWidth(COLUMN_WIDTH_FIXED) +
	GetSpacingWidth(SPACING_FIXED);

      assert(1 - wScale > EPSILON);
      tableWidth = std::max(tableWidth, fixedWidth / (1 - wScale));

      for (j = 0; j < nColumns; j++)
	if (column[j].widthType == COLUMN_WIDTH_PERCENTAGE)
	  column[j].width = tableWidth * column[j].scaleWidth;
      ConfirmHorizontalScaleSpacing(tableWidth);
	
      scaled delta = tableWidth -
	GetColumnWidth(COLUMN_WIDTH_PERCENTAGE) -
	GetColumnWidth(COLUMN_WIDTH_FIXED) -
	GetSpacingWidth();
      delta = std::max(scaled(0), delta);

      // if there are not % columns, then there is no need to reduce the
      // extra space to auto column so that their total width has a
      // give value. We set them all to the content width.
      float ratio = (constraint && avail != scaled(0)) ? delta.toFloat() / avail.toFloat() : 0;

      for (j = 0; j < nColumns; j++)
	if (column[j].widthType == COLUMN_WIDTH_AUTO)
	  column[j].width = column[j].contentWidth +
	    (column[j].width - column[j].contentWidth) * ratio;
    } else {
      for (j = 0; j < nColumns; j++)
	if (column[j].widthType == COLUMN_WIDTH_AUTO) column[j].width = column[j].contentWidth;

      for (j = 0; j < nColumns; j++) {
	if (column[j].widthType == COLUMN_WIDTH_FIT) {
	  ColumnLayout(j, FormattingContext(ctxt.GetLayoutType(), avail / static_cast<int>(n)));
	  avail = std::max(scaled(0), avail - std::max(column[j].contentWidth, column[j].width));
	  n--;
	}
      }
    }
  }

  SpannedCellsLayout(ctxt);
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
    ColumnLayout(j, FormattingContext(FormattingContext::LAYOUT_MIN, 0));
    column[j].minimumWidth = column[j].contentWidth;
  }

  for (i = 0; i < nRows; i++) {
    for (j = 0; j < nColumns; j++) {
      unsigned n = cell[i][j].colSpan;
      if (cell[i][j].mtd && !cell[i][j].spanned && n > 1) {
	scaled minWidth = GetMinimumWidth(j, cell[i][j].colSpan);
	cell[i][j].mtd->DoLayout(FormattingContext(FormattingContext::LAYOUT_MIN, 0));
	scaled cellWidth = cell[i][j].mtd->GetBoundingBox().width;
	if (cellWidth > minWidth) {
	  for (unsigned k = 0; k < n; k++) 
	    if (minWidth == scaled(0) || equalColumns)
	      column[j + k].minimumWidth = std::max(column[j + k].minimumWidth, cellWidth / static_cast<int>(n));
	    else
	      column[j + k].minimumWidth =
		column[j + k].minimumWidth * (cellWidth.toFloat() / minWidth.toFloat());
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
  if (frame != T_NONE && frameHorizontalSpacingType == id) counter += 2;
  for (unsigned j = 0; j + 1 < nColumns; j++) if (column[j].spacingType == id) counter++;
  return counter;
}

void
MathMLTableElement::ConfirmHorizontalFixedSpacing()
{
  if (frame != T_NONE) {
    if (frameHorizontalSpacingType == SPACING_FIXED)
      frameHorizontalSpacing = frameHorizontalFixedSpacing;
  } else
    frameHorizontalSpacing = 0;

  for (unsigned j = 0; j + 1 < nColumns; j++) {
    if (column[j].spacingType == SPACING_FIXED)
      {
	//cout << "confirming horizontal fixed spacing " << sp2ipx(column[j].fixedSpacing) << endl;
	column[j].spacing = column[j].fixedSpacing;
      }
  }
}

void
MathMLTableElement::ConfirmHorizontalScaleSpacing(const scaled& tableWidth)
{
  assert(tableWidth >= scaled(0));

  if (frame != T_NONE) {
    if (frameHorizontalSpacingType == SPACING_PERCENTAGE)
      frameHorizontalSpacing = tableWidth * frameHorizontalScaleSpacing;
  }

  for (unsigned j = 0; j + 1 < nColumns; j++) {
    if (column[j].spacingType == SPACING_PERCENTAGE)
      column[j].spacing = tableWidth * column[j].scaleSpacing;
  }
}

float
MathMLTableElement::GetHorizontalScale() const
{
  float scale = 0;
  unsigned j;

  if (frame != T_NONE)
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

  if (frame != T_NONE)
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
    if (k < n - 1) width += column[j + k].spacing;
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
MathMLTableElement::ColumnLayout(unsigned j, const FormattingContext& ctxt)
{
  scaled columnWidth = 0;

  for (unsigned i = 0; i < nRows; i++) {
    TableCell& tableCell = cell[i][j];
    if (tableCell.mtd &&
	!tableCell.spanned && tableCell.colSpan == 1) {

      if (ctxt.GetLayoutType() != FormattingContext::LAYOUT_AUTO || !tableCell.mtd->IsStretchyOperator())
	{
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
	  for (unsigned k = 0; k < tableCell.nAlignGroup; k++)
	    {
	      assert(tableCell.aGroup[k].group);
	      tableCell.aGroup[k].group->SetWidth(0);
	    }

	  if (tableCell.nAlignGroup > 0)
	    {
	      // since this cell is subject to alignment we cannot break its content
	      tableCell.mtd->DoLayout(FormattingContext(ctxt.GetLayoutType(), width));
	      tableCell.mtd->CalcGroupsExtent();
	    } 
	  else
	    tableCell.mtd->DoLayout(FormattingContext(ctxt.GetLayoutType(), width));
	}

      const BoundingBox& cellBox =
	tableCell.mtd->IsStretchyOperator() ?
	tableCell.mtd->GetBoundingBox() :
	tableCell.mtd->GetBoundingBox();

      if (cellBox.width > columnWidth) columnWidth = cellBox.width;
    }
  }

  columnWidth = std::max(columnWidth, ColumnGroupsLayout(j, ctxt));

  column[j].contentWidth = std::max(columnWidth, column[j].minimumWidth);
  column[j].width = std::max(column[j].contentWidth, width);
}

scaled
MathMLTableElement::ColumnGroupsLayout(unsigned j, const FormattingContext& ctxt)
{
  unsigned nAlignGroup = column[j].nAlignGroup;
  if (nAlignGroup == 0) return 0;

  GroupExtent* gExtent = (nAlignGroup > 0) ? new GroupExtent[nAlignGroup] : NULL;

  unsigned k;
  unsigned i;

  for (k = 0; k < nAlignGroup; k++)
    gExtent[k].left = gExtent[k].right = 0;

  for (i = 0; i < nRows; i++)
    {
      TableCell& tableCell = cell[i][j];
      if (tableCell.mtd &&
	  !tableCell.spanned &&
	  tableCell.colSpan == 1 &&
	  !tableCell.mtd->IsStretchyOperator()) 
	{
	  for (k = 0; k < tableCell.nAlignGroup; k++)
	    {
	      gExtent[k].left = std::max(gExtent[k].left, tableCell.aGroup[k].extent.left);
	      gExtent[k].right = std::max(gExtent[k].right, tableCell.aGroup[k].extent.right);
	    }
	}
    }

  scaled alignedCellWidth = 0;
  for (k = 0; k < nAlignGroup; k++)
    alignedCellWidth += gExtent[k].left + gExtent[k].right;

  if (ctxt.GetLayoutType() == FormattingContext::LAYOUT_AUTO)
    {
      for (i = 0; i < nRows; i++) {
	TableCell& tableCell = cell[i][j];
	if (tableCell.mtd &&
	    !tableCell.spanned &&
	    tableCell.colSpan == 1 &&
	    !tableCell.mtd->IsStretchyOperator())
	  {
	    for (k = 0; k < tableCell.nAlignGroup; k++)
	      {
		assert(tableCell.aGroup[k].group);

		scaled rightPrev = 0;
		if (k > 0) rightPrev = gExtent[k - 1].right - tableCell.aGroup[k - 1].extent.right;

		tableCell.aGroup[k].group->SetWidth(rightPrev + gExtent[k].left - tableCell.aGroup[k].extent.left);
		tableCell.aGroup[k].group->DoLayout(FormattingContext(FormattingContext::LAYOUT_AUTO, 0));
	      }
	  }

#if 0
	// ok, a little explanation: it is not necessary that every cell has exactly the same
	// number of alignment groups, furthermore it is almost impossibile that even
	// two cells with the same number of alignment groups have exaclty the same
	// width. Alignment groups acts in the innser part of the cell, but the rightmost
	// group has no right padding. However, we must be sure for every cell with alignment
	// groups to have the very same width, because of alignment purposes. If such cells are to be
	// aligned within their column, then they must have the same width to preserve
	// alignment inside groups.
	tableCell.mtd->RecalcBoundingBox(id, alignedCellWidth);
#endif
      }
    }
  
  delete [] gExtent;

  return alignedCellWidth;
}

void
MathMLTableElement::ScaleColumnsLayout(const FormattingContext& ctxt)
{
  for (unsigned j = 0; j < nColumns; j++)
    if (column[j].widthType == COLUMN_WIDTH_PERCENTAGE) {
      ColumnLayout(j, FormattingContext(ctxt.GetLayoutType(), tableWidth * column[j].scaleWidth));
    }
}

void
MathMLTableElement::SpannedCellsLayout(const FormattingContext& ctxt)
{
  for (unsigned i = 0; i < nRows; i++) {
    for (unsigned j = 0; j < nColumns; j++) {
      if (cell[i][j].mtd &&
	  !cell[i][j].spanned && cell[i][j].colSpan > 1) {
	if (ctxt.GetLayoutType() == FormattingContext::LAYOUT_MIN) {
	  cell[i][j].mtd->DoLayout(FormattingContext(FormattingContext::LAYOUT_MIN, 0));
	  const BoundingBox& cellBox = cell[i][j].mtd->GetBoundingBox();
	  scaled widthPerColumn = std::max(scaled(0), cellBox.width / static_cast<int>(cell[i][j].colSpan));
	  for (unsigned k = 0; k < cell[i][j].colSpan; k++)
	    column[j].minimumWidth = std::max(column[j].minimumWidth, widthPerColumn);
	} else {
	  scaled spannedWidth = GetColumnWidth(j, cell[i][j].colSpan);
	  cell[i][j].mtd->DoLayout(FormattingContext(ctxt.GetLayoutType(), spannedWidth));
	}
      }
    }
  }
}

void
MathMLTableElement::StretchyCellsLayout()
{
  for (unsigned i = 0; i < nRows; i++)
    {
      for (unsigned j = 0; j < nColumns; j++)
	{
	  if (cell[i][j].mtd && !cell[i][j].spanned)
	    {
	      SmartPtr<MathMLElement> cellElem = cell[i][j].mtd->GetChild();
	      assert(cellElem);
	      SmartPtr<MathMLOperatorElement> op = findStretchyOperator(cellElem);
	      if (op)
		{
		  scaled width = GetColumnWidth(j, cell[i][j].colSpan);

#if 0
		  if (op->GetStretch() == STRETCH_VERTICAL)
		    {
		      scaled height = GetRowHeight(i, cell[i][j].rowSpan);
		      op->VerticalStretchTo(row[i].height, height - row[i].height);
		    } 
		  else 
		    {
		      op->HorizontalStretchTo(width);
		    }
#endif

		  cell[i][j].mtd->DoLayout(FormattingContext(FormattingContext::LAYOUT_AUTO, width));
		}
	    }
	}
    }
}

void
MathMLTableElement::DoVerticalLayout(FormattingContext::LayoutId id)
{
  if (id == FormattingContext::LAYOUT_MIN) EnforceVerticalInvariants();

  ConfirmVerticalFixedSpacing();

  unsigned i;
  unsigned j;

  for (i = 0; i < nRows; i++) {
    scaled ascent = 0;
    scaled descent = 0;

    for (j = 0; j < nColumns; j++) 
      if (cell[i][j].mtd &&
	  !cell[i][j].spanned &&
	  cell[i][j].rowAlign == T_BASELINE) {
	const BoundingBox& box = cell[i][j].mtd->GetBoundingBox();
        ascent = std::max(ascent, box.height);
        if (cell[i][j].rowSpan == 1) descent = std::max(descent, box.depth);
      }

    if (HasLabels()) {
      if (rowLabel[i].labelElement != NULL &&
          rowLabel[i].rowAlign == T_BASELINE) {
        const BoundingBox& labelBox = rowLabel[i].labelElement->GetBoundingBox();
        ascent = std::max(ascent, labelBox.height);
        descent = std::max(descent, labelBox.depth);
      }
    }

    for (j = 0; j < nColumns; j++)
      if (cell[i][j].mtd &&
	  !cell[i][j].spanned && cell[i][j].rowSpan == 1 &&
	  cell[i][j].rowAlign != T_BASELINE) {
	const BoundingBox& box = cell[i][j].mtd->GetBoundingBox();
	if (box.verticalExtent() > ascent + descent) descent = box.verticalExtent() - ascent;
      }

    if (HasLabels()) {
      if (rowLabel[i].labelElement &&
	  rowLabel[i].rowAlign != T_BASELINE) {
	const BoundingBox& labelBox = rowLabel[i].labelElement->GetBoundingBox();
	if (labelBox.verticalExtent() > ascent + descent) descent = labelBox.verticalExtent() - ascent;
      }
    }

    row[i].height = ascent;
    row[i].depth = descent;
  }

  SpanRowHeight(id);

  if (equalRows) {
    scaled maxHeight = 0;
    for (i = 0; i < nRows; i++)
      maxHeight = std::max(maxHeight, row[i].verticalExtent());

    for (i = 0; i < nRows; i++)
      if (row[i].verticalExtent() < maxHeight)
	row[i].depth += maxHeight - row[i].verticalExtent();
  }

  scaled fixedHeight = GetRowHeight() + GetSpacingHeight(SPACING_FIXED);
  scaled tableHeight = fixedHeight / (1 - hScale);

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
  if (frame != T_NONE) {
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
MathMLTableElement::ConfirmVerticalScaleSpacing(const scaled& tableHeight)
{
  if (frame != T_NONE) {
    if (frameVerticalSpacingType == SPACING_PERCENTAGE)
      frameVerticalSpacing = tableHeight * frameVerticalScaleSpacing;
  }

  for (unsigned i = 0; i + 1 < nRows; i++) {
    if (row[i].spacingType == SPACING_PERCENTAGE)
      row[i].spacing = tableHeight * row[i].scaleSpacing;
  }
}

void
MathMLTableElement::SpanRowHeight(FormattingContext::LayoutId)
{
  for (unsigned j = 0; j < nColumns; j++)
    {
      unsigned i = 0;
      while (i < nRows)
	{
	  if (cell[i][j].mtd != NULL &&
	      !cell[i][j].spanned && cell[i][j].rowSpan > 1)
	    {
	      unsigned n = cell[i][j].rowSpan;
	      scaled height  = GetRowHeight(i, n);
	      const BoundingBox& cellBox = cell[i][j].mtd->GetBoundingBox();

	      if (height < cellBox.verticalExtent())
		{
		  // the total height of spanned rows is still smaller than the
		  // height of the single spanning cell. We have to distribute
		  // additional space among the spanned rows
		  scaled rest = cellBox.verticalExtent() - height;
		  //printf("column %d rest is %d\n", j, sp2ipx(rest));
		  for (unsigned k = 0; k < n; k++)
		    {
		      if (k == n - 1)
			// it is the last column, we assign all the remaining space
			row[i + k].depth += rest;
		      else
			{
			  // we assign a space proportional to the column height
			  scaled thisRowRest;
			  if (height != scaled(0))
			    // if all the columns have 0 width we assign an equal amount
			    // of space to each spanned column
			    thisRowRest = rest * (row[i + k].verticalExtent().toFloat() / height.toFloat());
			  else
			    thisRowRest = rest / static_cast<int>(n);
			  row[i + k].depth += thisRowRest;
			  rest -= thisRowRest;
			}
		    }
		}
	      i += n;
	    }
	  else
	    i++;
	}
    }
}
  
scaled
MathMLTableElement::GetRowHeight(unsigned i, unsigned n) const
{
  scaled height = 0;

  for (unsigned k = 0; k < n; k++) {
    height += row[i + k].verticalExtent();
    if (k < n - 1) height += row[i + k].spacing;
  }

  return height;
}

scaled
MathMLTableElement::GetRowHeight() const
{
  scaled height = 0;

  for (unsigned i = 0; i < nRows; i++)
    height += row[i].verticalExtent();

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
MathMLTableElement::AdjustTableWidth(const scaled& availWidth)
{
  unsigned j;
  scaled tableWidth = GetTableWidth();

  if (tableWidth <= availWidth) return;

  scaled extraSpace = std::max(scaled(0), tableWidth - GetContentWidth());
  if (extraSpace == scaled(0)) return;

  scaled toKill = std::max(scaled(0), std::min(tableWidth - availWidth, extraSpace));

  float ratio = 1 - toKill.toFloat() / extraSpace.toFloat();

  for (j = 0; j < nColumns; j++)
    column[j].width = column[j].contentWidth + (column[j].width - column[j].contentWidth) * ratio;

  frameHorizontalSpacing = std::max(MIN_COLUMN_SPACING, frameHorizontalSpacing * ratio);

  for (j = 0; j + 1 < nColumns; j++)
    if (column[j].spacing > MIN_COLUMN_SPACING)
      column[j].spacing = std::max(MIN_COLUMN_SPACING, column[j].spacing * ratio);
}

void
MathMLTableElement::AlignTable(const scaled& height, BoundingBox& box)
{
  if (rowNumber > static_cast<int>(nRows)) rowNumber = nRows;
  else if (-rowNumber > static_cast<int>(nRows)) rowNumber = -static_cast<int>(nRows);

  if (rowNumber < 0) rowNumber = nRows + rowNumber + 1;

  if (rowNumber == 0) {
    switch (align) {
    case T_TOP:
      box.height  = 0;
      break;
    case T_BOTTOM:
      box.height  = height;
      break;
    case T_AXIS:
      box.height  = height / 2 + environmentAxis;
      break;
    case T_CENTER:
    case T_BASELINE:
    default:
      box.height  = height / 2;
      break;
    }
  } else {
    scaled upTo = GetRowHeight(0, rowNumber);

    upTo += frameVerticalSpacing;

    switch (align) {
    case T_TOP:
      box.height = upTo - row[rowNumber - 1].verticalExtent();
      break;
    case T_BOTTOM:
      box.height = upTo;
      break;
    case T_AXIS:
      box.height = upTo - row[rowNumber - 1].verticalExtent() / 2 + environmentAxis;
      break;
    case T_BASELINE:
      box.height = upTo - row[rowNumber - 1].depth;
      break;
    case T_CENTER:
    default:
      box.height = upTo - row[rowNumber - 1].verticalExtent() / 2;
      break;      
    }
  }

  box.depth = height - box.height;
}

// PrepareLabelsLayout: this method is for deciding whether the labels
// have to be placed aside the table or if they will overlap. After
// deciding that, the method will return the effective available width
// to render the table.
scaled
MathMLTableElement::PrepareLabelsLayout(const FormattingContext& ctxt)
{
  // overlappingLabels = false;
  scaled aAvailWidth = ctxt.GetAvailableWidth();

  if (ctxt.GetLayoutType() == FormattingContext::LAYOUT_AUTO && HasLabels()) {
    // FIXME: what if the minLabelSpacing is a percentage value?
    assert(minLabelSpacingType == SPACING_FIXED);
    minLabelSpacing = std::max(scaled(0), minLabelFixedSpacing);
    
    scaled maxLabelWidth = GetMaxLabelWidth();
    
#if 0
    if (GetMaxBoundingBox().width - maxLabelWidth - minLabelSpacing <= availWidth &&
	availWidth < GetMaxBoundingBox().width &&
	(side == T_LEFTOVERLAP || side == T_RIGHTOVERLAP)) {
      // ok, there is not enough room to render the labels aside the
      // table, however is we drop the labels the table could fit well, so
      // the labels will overlap
      overlappingLabels = true;
    } else {
#endif
      // labels aside the table, compute the available space for the table itself
      aAvailWidth -= maxLabelWidth - minLabelSpacing;
      aAvailWidth = std::max(scaled(0), aAvailWidth);
#if 0
    }
#endif
  }

  return aAvailWidth;
}

scaled
MathMLTableElement::GetMaxLabelWidth() const
{  
  assert(rowLabel);

  scaled width = 0;
  for (unsigned i = 0; i < nRows; i++) {
    if (rowLabel[i].labelElement)
      width = std::max(width, rowLabel[i].labelElement->GetBoundingBox().width);
  }

  return width;
}

void
MathMLTableElement::DoLabelsLayout(const FormattingContext& ctxt)
{
  assert(rowLabel);

  scaled availForLabels = std::max(scaled(0), ctxt.GetAvailableWidth() - GetTableWidth() - minLabelFixedSpacing);

  for (unsigned i = 0; i < nRows; i++) {
    if (rowLabel[i].labelElement)
      rowLabel[i].labelElement->DoLayout(FormattingContext(ctxt.GetLayoutType(), availForLabels));
  }
}

void
MathMLTableElement::AdjustTableLayoutWithLabels(const FormattingContext& ctxt)
{
  assert(rowLabel);

  tableWidth = box.width;
  labelsWidth = 0;
  leftPadding = 0;

  for (unsigned i = 0; i < nRows; i++) {
    if (rowLabel[i].labelElement) {
      const BoundingBox& labelBox = rowLabel[i].labelElement->GetBoundingBox();
      labelsWidth = std::max(labelsWidth, labelBox.width);
    }
  }  

  tableWidth = box.width;

  if (ctxt.GetLayoutType() == FormattingContext::LAYOUT_AUTO &&
      ctxt.GetAvailableWidth() > labelsWidth + minLabelSpacing + tableWidth) {
    scaled extra = ctxt.GetAvailableWidth() - tableWidth;

    if (extra > (minLabelSpacing + labelsWidth) * 2) {
      leftPadding = extra / 2;
    } else {
      if (side == T_LEFT || side == T_LEFTOVERLAP)
	leftPadding = labelsWidth + minLabelSpacing;
      else
	leftPadding = extra - minLabelSpacing - labelsWidth;
    }

    box.width = std::max(ctxt.GetAvailableWidth(), tableWidth + labelsWidth + minLabelSpacing);
  } else {
    if (side == T_LEFT || side == T_LEFTOVERLAP)
      leftPadding = labelsWidth + minLabelSpacing;
    else
      leftPadding = 0;

    box.width += labelsWidth + minLabelSpacing;
  }
}
#endif
