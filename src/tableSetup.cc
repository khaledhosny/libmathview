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
#include <stdio.h>

#include "Array.hh"
#include "Iterator.hh"
#include "ValueConversion.hh"
#include "MathMLTableElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLTableCellElement.hh"

void
MathMLTableElement::Setup(RenderingEnvironment* env)
{
  ReleaseAuxStructures();
  assert(env != NULL);

  SetupCellSpanning(env);
  CalcTableSize();
  SetupCells();

  SetupAlignmentScopes(env);
  SetupColumns(env);
  SetupColumnAlign(env);

  SetupRows(env);
  SetupRowAlign(env);

  SetupGroups();
  SetupGroupAlign(env);

  SetupTableAttributes(env);
  SetupLabels();
  MathMLContainerElement::Setup(env);
  SetupAlignMarks();

#if 0
  for (unsigned i = 0; i < nRows; i++)
    for (unsigned j = 0; j < nColumns; j++) {
      printf("(%d,%d) rowAlign: %d columnAlign: %d\n", i, j, cell[i][j].rowAlign, cell[i][j].columnAlign);
    }
#endif

  color = env->GetColor();
  lineThickness = env->GetRuleThickness();
}

void
MathMLTableElement::SetupCellSpanning(RenderingEnvironment* env)
{
  for (Iterator<MathMLElement*> i(content); i.More(); i.Next()) {
    assert(i() != NULL);
    assert(i()->IsA() == TAG_MTR || i()->IsA() == TAG_MLABELEDTR);

    MathMLTableRowElement* mtr = TO_TABLEROW(i());
    assert(mtr != NULL);

    mtr->SetupCellSpanning(env);
  }
}

// CalcTableSize: requires normalization and spanning attributes
// compute total number of rows and columns within this table
void
MathMLTableElement::CalcTableSize()
{
  unsigned i;

  nRows = content.GetSize();
  nColumns = 0;

  if (nRows == 0) return;

  class TempRow {
  public:
    TempRow(void)
    {
      first = 0;
    }

    void AddSpanningCell(unsigned j, unsigned n = 1)
    {
      assert(n >= 1);

      if (j + n > content.GetSize()) content.SetSize(j + n, false);
      for (unsigned k = 0; k < n; k++) content.Set(j + k, true);
    }

    unsigned AddCell(unsigned n)
    {
      assert(n >= 1);

      unsigned j = first;
      unsigned k = 0;
      while (k < n && j < content.GetSize()) {
	for (k = 0; k < n && j + k < content.GetSize() && !content.Get(j + k); k++) ;
	if (k < n) j += k + 1;
      }

      if (j >= content.GetSize()) content.SetSize(j + n, false);
      for (k = 0; k < n; k++) content.Set(j + k, true);
      first = j + n;

      return j;
    }

    unsigned GetColumns(void) const
    {
      return (first > content.GetSize()) ? first : content.GetSize();
    }

  private:
    unsigned    first;
    Array<bool> content;
  };


  TempRow* r = new TempRow[nRows];

  i = 0;
  for (Iterator<MathMLElement*> p(content); p.More(); p.Next()) {
    assert(p() != NULL);
    assert(p()->IsA() == TAG_MTR || p()->IsA() == TAG_MLABELEDTR);

    MathMLTableRowElement* mtr = TO_TABLEROW(p());
    assert(mtr != NULL);

    Iterator<MathMLElement*> q(mtr->content);
    if (mtr->IsA() == TAG_MLABELEDTR) {
      assert(q.More());
      q.Next();
    }

    while (q.More()) {
      assert(q() != NULL);
      assert(q()->IsA() == TAG_MTD);

      MathMLTableCellElement* mtd = TO_TABLECELL(q());
      assert(mtd != NULL);

      unsigned j = r[i].AddCell(mtd->GetColumnSpan());

      for (unsigned k = 1; k < mtd->GetRowSpan(); k++) {
	r[i + k].AddSpanningCell(j, mtd->GetColumnSpan());
      }

      mtd->SetupCellPosition(i, j, nRows);

      q.Next();
    }

    i++;
  }

  for (i = 0; i < nRows; i++) if (r[i].GetColumns() > nColumns) nColumns = r[i].GetColumns();

  delete [] r;
}

// create a matrix of TableCells for easy access to the table's elements
void
MathMLTableElement::SetupCells()
{
  if (nRows == 0 || nColumns == 0) return;

  cell = new TableCellPtr[nRows];
  for (unsigned i = 0; i < nRows; i++) {
    cell[i] = new TableCell[nColumns];

    for (unsigned j = 0; j < nColumns; j++) {
      cell[i][j].mtd = NULL;
      cell[i][j].rowSpan = 0;
      cell[i][j].colSpan = 0;
      cell[i][j].spanned = false;
      // following init is necessary (see SetupColumnAlign)
      cell[i][j].columnAlign = COLUMN_ALIGN_NOTVALID;
    }
  }

  for (Iterator<MathMLElement*> p(content); p.More(); p.Next()) {
    assert(p() != NULL);
    assert(p()->IsA() == TAG_MTR || p()->IsA() == TAG_MLABELEDTR);

    MathMLTableRowElement* mtr = TO_TABLEROW(p());
    assert(mtr != NULL);

    Iterator<MathMLElement*> q(mtr->content);
    if (mtr->IsA() == TAG_MLABELEDTR) {
      assert(q.More());
      q.Next();
    }

    while (q.More()) {
      assert(q() != NULL);
      assert(q()->IsA() == TAG_MTD);

      MathMLTableCellElement* mtd = TO_TABLECELL(q());
      assert(mtd != NULL);

      unsigned i0 = mtd->GetRowIndex();
      unsigned j0 = mtd->GetColumnIndex();
      unsigned n = mtd->GetRowSpan();
      unsigned m = mtd->GetColumnSpan();

      mtd->SetupCell(&cell[i0][j0]);

      for (unsigned i = 0; i < n; i++)
	for (unsigned j = 0; j < m; j++) {
	  assert(cell[i0 + i][j0 + j].mtd == NULL);
	  cell[i0 + i][j0 + j].mtd = mtd;
	  cell[i0 + i][j0 + j].rowSpan = n - i;
	  cell[i0 + i][j0 + j].colSpan = m - j;
	  cell[i0 + i][j0 + j].spanned = (i > 0) || (j > 0);
	}

      q.Next();
    }
  }
}

void
MathMLTableElement::SetupColumns(RenderingEnvironment* env)
{
  if (nColumns == 0) return;
  unsigned i = 0;

  column = new TableColumn[nColumns];
  for (unsigned j = 0; j < nColumns; j++) {
    column[j].widthType = COLUMN_WIDTH_NOTVALID;
    column[j].fixedWidth = 0;
    column[j].spacingType = SPACING_NOTVALID;
    column[j].fixedSpacing = 0;
    column[j].lineType = TABLE_LINE_NOTVALID;
  }

  const Value* value = NULL;

  value = GetAttributeValue(ATTR_COLUMNWIDTH, env);

  for (i = 0; i < nColumns; i++) {
    const Value* v = Resolve(value, env, i);
    assert(v != NULL);

    if      (v->IsKeyword(KW_AUTO)) column[i].widthType = COLUMN_WIDTH_AUTO;
    else if (v->IsKeyword(KW_FIT))  column[i].widthType = COLUMN_WIDTH_FIT;
    else {
      assert(v->IsNumberUnit());

      UnitValue unitValue = v->ToNumberUnit();

      if (unitValue.IsPercentage()) {
	column[i].widthType  = COLUMN_WIDTH_PERCENTAGE;
	column[i].scaleWidth = unitValue.GetValue();
      } else {
	column[i].widthType  = COLUMN_WIDTH_FIXED;
	column[i].fixedWidth = env->ToScaledPoints(unitValue);
      }
    }

    delete v;
  }
  
  delete value;

  value = GetAttributeValue(ATTR_COLUMNSPACING, env);

  for (i = 0; i < nColumns; i++) {
    const Value* v = Resolve(value, env, i);
    assert(v->IsNumberUnit());

    UnitValue unitValue = v->ToNumberUnit();

    if (unitValue.IsPercentage()) {
      column[i].spacingType  = SPACING_PERCENTAGE;
      column[i].scaleSpacing = unitValue.GetValue();
    } else {
      column[i].spacingType  = SPACING_FIXED;
      column[i].fixedSpacing = env->ToScaledPoints(unitValue);
    }

    delete v;
  }

  delete value;
}

void
MathMLTableElement::SetupAlignmentScopes(RenderingEnvironment* env)
{
  const Value* value = GetAttributeValue(ATTR_ALIGNMENTSCOPE, env);
  assert(value != NULL);

  for (unsigned j = 0; j < nColumns; j++) {
    const Value* p = value->Get(j);
    assert(p != NULL && p->IsBoolean());
    for (unsigned i = 0; i < nRows; i++)
      if (cell[i][j].mtd != NULL)
	cell[i][j].mtd->SetAlignmentScope(p->ToBoolean());
  }

  delete value;
}

void
MathMLTableElement::SetupColumnAlign(RenderingEnvironment* env)
{
  const Value* value = GetAttributeValue(ATTR_COLUMNALIGN, env);
  SetupColumnAlignAux(value, 0, nRows);
  delete value;
}

void
MathMLTableElement::SetupColumnAlignAux(const Value* value,
					unsigned rowStart,
					unsigned n,
					bool labeledRow)
{
  assert(rowStart < nRows);
  assert(n <= nRows);
  assert(rowStart + n <= nRows);
  assert(value != NULL);

  unsigned j0 = labeledRow ? 1 : 0;

  for (unsigned j = 0; j < nColumns + j0; j++) {
    const Value* p = value->Get(j);
    assert(p != NULL);

    ColumnAlignId columnAlign = ToColumnAlignId(p);

    if (labeledRow && j == 0) {
      assert(rowLabel != NULL);
      assert(n == 1);

      rowLabel[rowStart].columnAlign = columnAlign;
    } else {
      // BEWARE: cells with alignment groups must all be aligned with the
      // the same alignment given in the mtable element. The kind of
      // alignment cannot be overridden by inner elements (mtr, mtd)
      for (unsigned i = 0; i < n; i++)
	if (cell[rowStart + i][j - j0].columnAlign == COLUMN_ALIGN_NOTVALID ||
	    cell[rowStart + i][j - j0].nAlignGroup == 0)
	  cell[rowStart + i][j - j0].columnAlign = columnAlign;
    }
  }
}

void
MathMLTableElement::SetupRows(RenderingEnvironment* env)
{
  if (nRows == 0) return;
  unsigned i = 0;

  row = new TableRow[nRows];
  for (i = 0; i < nRows; i++) {
    row[i].mtr = NULL;
    row[i].spacingType = SPACING_NOTVALID;
    row[i].fixedSpacing = 0;
    row[i].lineType = TABLE_LINE_NOTVALID;
  }

  i = 0;
  for (Iterator<MathMLElement*> rowElem(content); rowElem.More(); rowElem.Next()) {
    assert(i < nRows);
    assert(rowElem()->IsA() == TAG_MTR || rowElem()->IsA() == TAG_MLABELEDTR);

    MathMLTableRowElement* mtr = TO_TABLEROW(rowElem());
    assert(mtr != NULL);

    row[i].mtr = mtr;
    mtr->SetupRowIndex(i);

    i++;
  }

  const Value* value = GetAttributeValue(ATTR_ROWSPACING, env);
  assert(value != NULL);

  for (i = 0; i < nRows; i++) {
    const Value* p = value->Get(i);
    assert(p != NULL && p->IsNumberUnit());

    UnitValue unitValue = p->ToNumberUnit();

    if (unitValue.IsPercentage()) {
      row[i].spacingType  = SPACING_PERCENTAGE;
      row[i].scaleSpacing = unitValue.GetValue();
    } else {
      row[i].spacingType  = SPACING_FIXED;
      row[i].fixedSpacing = env->ToScaledPoints(unitValue);
    }
  }

  delete value;
}

void
MathMLTableElement::SetupRowAlign(RenderingEnvironment* env)
{
  const Value* value = GetAttributeValue(ATTR_ROWALIGN, env);
  assert(value != NULL);

  for (unsigned i = 0; i < nRows; i++) {
    const Value* p = value->Get(i);
    SetupRowAlignAux(p, i);
  }

  delete value;
}

void
MathMLTableElement::SetupRowAlignAux(const Value* value,
				     unsigned i,
				     bool labeledRow)
{
  assert(value != NULL);
  assert(i < nRows);

  RowAlignId rowAlign = ToRowAlignId(value);

  if (labeledRow) {
    assert(rowLabel != NULL);
    rowLabel[i].rowAlign = rowAlign;
  }

  for (unsigned j = 0; j < nColumns; j++)
    cell[i][j].rowAlign = rowAlign;
}

void
MathMLTableElement::SetupLabels()
{
  unsigned i = 0;

  if (rowLabel != NULL) {
    delete rowLabel;
    rowLabel = NULL;
  }

  bool hasLabels = false;
  for (i = 0; i < nRows && !hasLabels; i++) {
    assert(row[i].mtr != NULL);
    hasLabels = (row[i].mtr->IsA() == TAG_MLABELEDTR);
  }

  if (!hasLabels) return;

  rowLabel = new RowLabel[nRows];
  for (i = 0; i < nRows; i++) {
    assert(row[i].mtr != NULL);
    rowLabel[i].labelElement = row[i].mtr->GetLabel();
    if (side == TABLE_SIDE_LEFT || side == TABLE_SIDE_LEFTOVERLAP)
      rowLabel[i].columnAlign = COLUMN_ALIGN_LEFT;
    else
      rowLabel[i].columnAlign = COLUMN_ALIGN_RIGHT;
    rowLabel[i].rowAlign = ROW_ALIGN_BASELINE;
  }
}

void
MathMLTableElement::SetupGroups()
{
  for (unsigned j = 0; j < nColumns; j++) {
    column[j].nAlignGroup = 0;

    for (unsigned i = 0; i < nRows; i++) {
      cell[i][j].iGroup      = 0;
      cell[i][j].nAlignGroup = 0;
      cell[i][j].group       = NULL;
      cell[i][j].aGroup      = NULL;

      if (!cell[i][j].spanned && cell[i][j].mtd != NULL) {
	MathMLTableCellElement::SetupGroups(cell[i][j].mtd->content.GetFirst(),
					    true, true,
					    cell[i][j]);

	if (cell[i][j].nAlignGroup > column[j].nAlignGroup) 
	  column[j].nAlignGroup = cell[i][j].nAlignGroup;

	if (cell[i][j].nAlignGroup > 0) {
	  AlignmentGroup* aGroup = new AlignmentGroup[cell[i][j].nAlignGroup];

	  for (unsigned k = 0; k < cell[i][j].nAlignGroup; k++) {
	    aGroup[k].group = NULL;
	    aGroup[k].alignment = GROUP_ALIGN_NOTVALID;
	    aGroup[k].extent.left = aGroup[k].extent.right = 0;
	  }

	  cell[i][j].aGroup = aGroup;
	}
      }
    }
  }
}

void
MathMLTableElement::SetupGroupAlign(RenderingEnvironment* env)
{
  const Value* value = GetAttributeValue(ATTR_GROUPALIGN, env);
  SetupGroupAlignAux(value, 0, nRows);
  delete value;
}

void
MathMLTableElement::SetupGroupAlignAux(const Value* value,
				       unsigned rowStart,
				       unsigned n)
{
  assert(value != NULL);

  for (unsigned j = 0; j < nColumns; j++) {
    for (unsigned k = 0; k < column[j].nAlignGroup; k++) {
      const Value* p = value->Get(j, k);
      assert(p != NULL);

      GroupAlignId groupAlignment = ToGroupAlignId(p);

      for (unsigned i = 0; i + 1 <= n; i++) {
	if (!cell[rowStart + i][j].spanned && k < cell[rowStart + i][j].nAlignGroup) {
	  cell[rowStart + i][j].aGroup[k].alignment = groupAlignment;
	}
      }
    }
  }
}

void
MathMLTableElement::SetupAlignMarks()
{
  for (unsigned i = 0; i < nRows; i++)
    for (unsigned j = 0; j < nColumns; j++)
      if (!cell[i][j].spanned && cell[i][j].mtd != NULL) {
	MathMLTableCellElement::SetupGroups(cell[i][j].mtd->content.GetFirst(),
					    true, false,
					    cell[i][j]);
      }
}

// finally, setup any attribute relative to the table itself and not
// any sub-element
void
MathMLTableElement::SetupTableAttributes(RenderingEnvironment* env)
{
  const Value* value = NULL;
  const Value* p = NULL;
  UnitValue unitValue;

  // align

  value = GetAttributeValue(ATTR_ALIGN, env);
  assert(value != NULL);

  p = value->Get(0);
  assert(p != NULL);

  if      (p->IsKeyword(KW_TOP)) align = TABLE_ALIGN_TOP;
  else if (p->IsKeyword(KW_BOTTOM)) align = TABLE_ALIGN_BOTTOM;
  else if (p->IsKeyword(KW_CENTER)) align = TABLE_ALIGN_CENTER;
  else if (p->IsKeyword(KW_BASELINE)) align = TABLE_ALIGN_BASELINE;
  else if (p->IsKeyword(KW_AXIS)) {
    align = TABLE_ALIGN_AXIS;
    environmentAxis = env->GetAxis();
  } else assert(IMPOSSIBLE);

  p = value->Get(1);
  assert(p != NULL);

  if (p->IsEmpty()) rowNumber = 0;
  else rowNumber = p->ToInteger();

  delete value;

  // rowlines

  value = GetAttributeValue(ATTR_ROWLINES, env);
  assert(value != NULL);

  for (unsigned i = 0; i < nRows; i++) {
    p = value->Get(i);
    assert(p != NULL);

    row[i].lineType = ToLineId(p);
  }

  delete value;

  // columnlines

  value = GetAttributeValue(ATTR_COLUMNLINES, env);
  assert(value != NULL);

  for (unsigned j = 0; j < nColumns; j++) {
    p = value->Get(j);
    assert(p != NULL);

    column[j].lineType = ToLineId(p);
  }

  delete value;

  // frame

  value = GetAttributeValue(ATTR_FRAME, env);
  assert(value != NULL);
  frame = ToLineId(value);
  delete value;

  // width

  value = GetAttributeValue(ATTR_WIDTH, env);
  assert(value != NULL);

  if (value->IsKeyword(KW_AUTO)) {
    widthType = WIDTH_AUTO;
  } else {
    assert(value->IsNumberUnit());
    unitValue = value->ToNumberUnit();
    if (unitValue.IsPercentage()) {
      widthType = WIDTH_PERCENTAGE;
      scaleWidth = unitValue.GetValue();
    } else {
      widthType = WIDTH_FIXED;
      fixedWidth = env->ToScaledPoints(unitValue);
    }
  }

  delete value;

  // framespacing

  value = GetAttributeValue(ATTR_FRAMESPACING, env);
  assert(value != NULL);

  p = Resolve(value, env, 0);
  assert(p != NULL && p->IsNumberUnit());

  unitValue = p->ToNumberUnit();
  if (unitValue.IsPercentage()) {
    frameHorizontalSpacingType  = SPACING_PERCENTAGE;
    frameHorizontalScaleSpacing = unitValue.GetValue();
  } else {
    frameHorizontalSpacingType  = SPACING_FIXED;
    frameHorizontalFixedSpacing = env->ToScaledPoints(unitValue);
  }

  delete p;

  p = Resolve(value, env, 1);
  assert(p != NULL && p->IsNumberUnit());

  unitValue = p->ToNumberUnit();
  if (unitValue.IsPercentage()) {
    frameVerticalSpacingType  = SPACING_PERCENTAGE;
    frameVerticalScaleSpacing = unitValue.GetValue();
  } else {
    frameVerticalSpacingType  = SPACING_FIXED;
    frameVerticalFixedSpacing = env->ToScaledPoints(unitValue);
  }

  delete p;

  if (frame == TABLE_LINE_NONE) {
    frameHorizontalSpacingType = frameVerticalSpacingType = SPACING_FIXED;
    frameHorizontalFixedSpacing = frameVerticalFixedSpacing = 0;
  }

  delete value;

  // equalrows

  value = GetAttributeValue(ATTR_EQUALROWS, env);
  assert(value != NULL && value->IsBoolean());
  equalRows = value->ToBoolean();
  delete value;

  // equalcolumns

  value = GetAttributeValue(ATTR_EQUALCOLUMNS, env);
  assert(value != NULL && value->IsBoolean());
  equalColumns = value->ToBoolean();
  delete value;

  // displaystyle

  value = GetAttributeValue(ATTR_DISPLAYSTYLE, env);
  assert(value != NULL && value->IsBoolean());
  displayStyle = value->ToBoolean();
  delete value;

  // side

  value = GetAttributeValue(ATTR_SIDE, env);
  assert(value != NULL);

  if      (value->IsKeyword(KW_LEFT)) side = TABLE_SIDE_LEFT;
  else if (value->IsKeyword(KW_RIGHT)) side = TABLE_SIDE_RIGHT;
  else if (value->IsKeyword(KW_LEFTOVERLAP)) side = TABLE_SIDE_LEFTOVERLAP;
  else if (value->IsKeyword(KW_RIGHTOVERLAP)) side = TABLE_SIDE_RIGHTOVERLAP;
  else assert(IMPOSSIBLE);

  delete value;

  // minlabelspacing

  value = GetAttributeValue(ATTR_MINLABELSPACING, env);
  assert(value != NULL && value->IsNumberUnit());

  unitValue = value->ToNumberUnit();
  if (unitValue.IsPercentage()) {
    minLabelSpacingType  = SPACING_PERCENTAGE;
    minLabelScaleSpacing = unitValue.GetValue();
  } else {
    minLabelSpacingType  = SPACING_FIXED;
    minLabelFixedSpacing = env->ToScaledPoints(unitValue);
  }

  delete value;
}

void
MathMLTableElement::ReleaseAuxStructures()
{
  if (row != NULL) {
    delete [] row;
    row = NULL;
  }

  if (column != NULL) {
    delete [] column;
    column = NULL;
  }

  if (cell != NULL) {
    for (unsigned i = 0; i < nRows; i++) {
      for (unsigned j = 0; j < nColumns; j++) {
	delete [] cell[i][j].aGroup;
      }
      delete [] cell[i];
    }

    delete [] cell;
    cell = NULL;
  }
}
