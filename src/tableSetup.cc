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

#include <vector>

#include "defs.h"
#include "ValueConversion.hh"
#include "MathMLTableElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLTableCellElement.hh"
#include "MathMLLabeledTableRowElement.hh"

void
MathMLTableElement::Setup(RenderingEnvironment& env)
{
  //printf("redoing table setup %p %d %d\n", this, (DirtyAttribute() || DirtyAttributeP()), (row ? sp2ipx(row[0].GetHeight()) : -1));
  if (DirtyAttribute() || DirtyAttributeP())
    {
      color = env.GetColor();
      lineThickness = env.GetRuleThickness();

      ReleaseAuxStructures();

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
      MathMLLinearContainerElement::Setup(env);
      SetupAlignMarks();

#if 0
      for (unsigned i = 0; i < nRows; i++)
	for (unsigned j = 0; j < nColumns; j++) {
	  printf("(%d,%d) rowAlign: %d columnAlign: %d\n", i, j, cell[i][j].rowAlign, cell[i][j].columnAlign);
	}
#endif
      ResetDirtyAttribute();
      SetDirtyLayout(); // DIRTY PATCH!
    }
  //printf("done table setup %p %d %d\n", this, (DirtyAttribute() || DirtyAttributeP()), (row ? sp2ipx(row[0].GetHeight()) : -1));
}

void
MathMLTableElement::SetupCellSpanning(RenderingEnvironment& env)
{
  for (std::vector< SmartPtr<MathMLElement> >::const_iterator i = GetContent().begin();
       i != GetContent().end();
       i++)
    {
      assert(*i);
      assert(is_a<MathMLTableRowElement>(*i));

      SmartPtr<MathMLTableRowElement> mtr = smart_cast<MathMLTableRowElement>(*i);
      assert(mtr);

      mtr->SetupCellSpanning(env);
    }
}

// BEGIN OF PRIVATE CLASS
class TempRow {
public:
  TempRow(void)
  {
    first = 0;
  }

  void AddSpanningCell(unsigned j, unsigned n = 1)
  {
    assert(n >= 1);

    if (j + n > content.size()) content.resize(j + n, false);
    for (unsigned k = 0; k < n; k++) content[j + k] = true;
  }

  unsigned AddCell(unsigned n)
  {
    assert(n >= 1);

    unsigned j = first;
    unsigned k = 0;
    while (k < n && j < content.size()) {
      for (k = 0; k < n && j + k < content.size() && !content[j + k]; k++) ;
      if (k < n) j += k + 1;
    }

    if (j >= content.size()) content.resize(j + n, false);
    for (k = 0; k < n; k++) content[j + k] = true;
    first = j + n;

    return j;
  }

  unsigned GetColumns(void) const
  {
    return (first > content.size()) ? first : content.size();
  }

private:
  unsigned first;
  std::vector<bool> content;
};
// END OF PRIVATE CLASS

// CalcTableSize: requires normalization and spanning attributes
// compute total number of rows and columns within this table
void
MathMLTableElement::CalcTableSize()
{
  unsigned i;

  nRows = GetSize();
  nColumns = 0;

  if (nRows == 0) return;

  std::vector<TempRow> r(nRows);

  i = 0;
  for (std::vector< SmartPtr<MathMLElement> >::const_iterator p = GetContent().begin();
       p != GetContent().end();
       p++)
    {
      assert(*p);
      assert(is_a<MathMLTableRowElement>(*p));

      SmartPtr<MathMLTableRowElement> mtr = smart_cast<MathMLTableRowElement>(*p);
      assert(mtr);

      std::vector< SmartPtr<MathMLElement> >::const_iterator q = mtr->GetContent().begin();
      if (is_a<MathMLLabeledTableRowElement>(mtr))
	{
	  assert(q != mtr->GetContent().end());
	  q++;
	}

      while (q != mtr->GetContent().end())
	{
	  assert(*q);
	  assert(is_a<MathMLTableCellElement>(*q));

	  SmartPtr<MathMLTableCellElement> mtd = smart_cast<MathMLTableCellElement>(*q);
	  assert(mtd);

	  unsigned j = r[i].AddCell(mtd->GetColumnSpan());

	  for (unsigned k = 1; k < mtd->GetRowSpan(); k++)
	    {
	      r[i + k].AddSpanningCell(j, mtd->GetColumnSpan());
	    }

	  mtd->SetupCellPosition(i, j, nRows);

	  q++;
	}

      i++;
    }

  for (i = 0; i < nRows; i++) if (r[i].GetColumns() > nColumns) nColumns = r[i].GetColumns();
}

// create a matrix of TableCells for easy access to the table's elements
void
MathMLTableElement::SetupCells()
{
  if (nRows == 0 || nColumns == 0) return;

  cell = new TableCellPtr[nRows];
  for (unsigned i = 0; i < nRows; i++) cell[i] = new TableCell[nColumns];

  for (std::vector< SmartPtr<MathMLElement> >::const_iterator p = GetContent().begin();
       p != GetContent().end();
       p++)
    {
      assert(*p);
      assert(is_a<MathMLTableRowElement>(*p));

      SmartPtr<MathMLTableRowElement> mtr = smart_cast<MathMLTableRowElement>(*p);
      assert(mtr);

      std::vector< SmartPtr<MathMLElement> >::const_iterator q = mtr->GetContent().begin();
      if (is_a<MathMLLabeledTableRowElement>(mtr))
	{
	  assert(q != mtr->GetContent().end());
	  q++;
	}

      while (q != mtr->GetContent().end())
	{
	  assert(*q);
	  assert(is_a<MathMLTableCellElement>(*q));

	  SmartPtr<MathMLTableCellElement> mtd = smart_cast<MathMLTableCellElement>(*q);
	  assert(mtd);

	  unsigned i0 = mtd->GetRowIndex();
	  unsigned j0 = mtd->GetColumnIndex();
	  unsigned n = mtd->GetRowSpan();
	  unsigned m = mtd->GetColumnSpan();

	  mtd->SetupCell(&cell[i0][j0]);

	  for (unsigned i = 0; i < n; i++)
	    for (unsigned j = 0; j < m; j++) 
	      {
		assert(!cell[i0 + i][j0 + j].mtd);
		cell[i0 + i][j0 + j].mtd = mtd;
		cell[i0 + i][j0 + j].rowSpan = n - i;
		cell[i0 + i][j0 + j].colSpan = m - j;
		cell[i0 + i][j0 + j].spanned = (i > 0) || (j > 0);
	      }

	  q++;
	}
    }
}

void
MathMLTableElement::SetupColumns(RenderingEnvironment& env)
{
  if (nColumns == 0) return;
  unsigned i = 0;

  column = new TableColumn[nColumns];

  SmartPtr<Value> value = GetAttributeValue(ATTR_COLUMNWIDTH, env);

  for (i = 0; i < nColumns; i++)
    {
      SmartPtr<Value> v = Resolve(value, env, i);
      assert(v);

      if (IsKeyword(v))
	switch (ToKeywordId(v))
	  {
	  case KW_AUTO: column[i].widthType = COLUMN_WIDTH_AUTO; break;
	  case KW_FIT: column[i].widthType = COLUMN_WIDTH_FIT; break;
	  default: assert(IMPOSSIBLE); break;
	  }
      else
	{
	  assert(IsNumberUnit(v));
	
	  UnitValue unitValue = ToNumberUnit(v);

	  if (unitValue.IsPercentage())
	    {
	      column[i].widthType  = COLUMN_WIDTH_PERCENTAGE;
	      column[i].scaleWidth = unitValue.GetValue();
	    } 
	  else
	    {
	      column[i].widthType  = COLUMN_WIDTH_FIXED;
	      column[i].fixedWidth = env.ToScaledPoints(unitValue);
	    }
	}
    }

  value = GetAttributeValue(ATTR_COLUMNSPACING, env);

  for (i = 0; i < nColumns; i++)
    {
      SmartPtr<Value> v = Resolve(value, env, i);
      assert(IsNumberUnit(v));

      UnitValue unitValue = ToNumberUnit(v);

      if (unitValue.IsPercentage())
	{
	  column[i].spacingType  = SPACING_PERCENTAGE;
	  column[i].scaleSpacing = unitValue.GetValue();
	} 
      else
	{
	  column[i].spacingType  = SPACING_FIXED;
	  column[i].fixedSpacing = env.ToScaledPoints(unitValue);
	}
    }
}

void
MathMLTableElement::SetupAlignmentScopes(RenderingEnvironment& env)
{
  SmartPtr<Value> value = GetAttributeValue(ATTR_ALIGNMENTSCOPE, env);
  assert(value);

  for (unsigned j = 0; j < nColumns; j++)
    {
      SmartPtr<Value> p = GetComponent(value, j);
      assert(p);
      for (unsigned i = 0; i < nRows; i++)
	if (cell[i][j].mtd)
	  cell[i][j].mtd->SetAlignmentScope(ToBoolean(p));
    }
}

void
MathMLTableElement::SetupColumnAlign(RenderingEnvironment& env)
{
  SmartPtr<Value> value = GetAttributeValue(ATTR_COLUMNALIGN, env);
  SetupColumnAlignAux(value, 0, nRows);
}

void
MathMLTableElement::SetupColumnAlignAux(const SmartPtr<Value>& value,
					unsigned rowStart,
					unsigned n,
					bool labeledRow)
{
  assert(rowStart < nRows);
  assert(n <= nRows);
  assert(rowStart + n <= nRows);
  assert(value);

  unsigned j0 = labeledRow ? 1 : 0;

  for (unsigned j = 0; j < nColumns + j0; j++)
    {
      SmartPtr<Value> p = GetComponent(value, j);
      assert(p);

      ColumnAlignId columnAlign = ToColumnAlignId(p);
      
      if (labeledRow && j == 0)
	{
	  assert(rowLabel);
	  assert(n == 1);
	  
	  rowLabel[rowStart].columnAlign = columnAlign;
	} 
      else
	{
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
MathMLTableElement::SetupRows(RenderingEnvironment& env)
{
  if (nRows == 0) return;
  unsigned i = 0;

  row = new TableRow[nRows];

  i = 0;
  for (std::vector< SmartPtr<MathMLElement> >::const_iterator rowElem = GetContent().begin();
       rowElem != GetContent().end();
       rowElem++)
    {
      assert(i < nRows);
      assert(is_a<MathMLTableRowElement>(*rowElem));

      SmartPtr<MathMLTableRowElement> mtr = smart_cast<MathMLTableRowElement>(*rowElem);
      assert(mtr);

      row[i].mtr = mtr;
      mtr->SetupRowIndex(i);

      i++;
    }

  SmartPtr<Value> value = GetAttributeValue(ATTR_ROWSPACING, env);
  assert(value);

  for (i = 0; i < nRows; i++)
    {
      SmartPtr<Value> p = GetComponent(value, i);
      assert(p && IsNumberUnit(p));

      UnitValue unitValue = ToNumberUnit(p);

      if (unitValue.IsPercentage())
	{
	  row[i].spacingType  = SPACING_PERCENTAGE;
	  row[i].scaleSpacing = unitValue.GetValue();
	} 
      else
	{
	  row[i].spacingType  = SPACING_FIXED;
	  row[i].fixedSpacing = env.ToScaledPoints(unitValue);
	}
    }
}

void
MathMLTableElement::SetupRowAlign(RenderingEnvironment& env)
{
  SmartPtr<Value> value = GetAttributeValue(ATTR_ROWALIGN, env);
  assert(value);

  for (unsigned i = 0; i < nRows; i++)
    {
      SmartPtr<Value> p = GetComponent(value, i);
      SetupRowAlignAux(p, i);
    }
}

void
MathMLTableElement::SetupRowAlignAux(const SmartPtr<Value>& value,
				     unsigned i,
				     bool labeledRow)
{
  assert(value);
  assert(i < nRows);

  RowAlignId rowAlign = ToRowAlignId(value);

  if (labeledRow)
    {
      assert(rowLabel);
      rowLabel[i].rowAlign = rowAlign;
    }

  for (unsigned j = 0; j < nColumns; j++)
    cell[i][j].rowAlign = rowAlign;
}

void
MathMLTableElement::SetupLabels()
{
  if (rowLabel)
    {
      delete rowLabel;
      rowLabel = 0;
    }

  bool hasLabels = false;
  for (unsigned i = 0; i < nRows && !hasLabels; i++)
    {
      assert(row[i].mtr);
      hasLabels = is_a<MathMLLabeledTableRowElement>(row[i].mtr);
    }

  if (hasLabels)
    {
      rowLabel = new RowLabel[nRows];
      for (unsigned i = 0; i < nRows; i++)
	{
	  assert(row[i].mtr);
	  rowLabel[i].labelElement = row[i].mtr->GetLabel();
	  if (side == TABLE_SIDE_LEFT || side == TABLE_SIDE_LEFTOVERLAP)
	    rowLabel[i].columnAlign = COLUMN_ALIGN_LEFT;
	  else
	    rowLabel[i].columnAlign = COLUMN_ALIGN_RIGHT;
	  rowLabel[i].rowAlign = ROW_ALIGN_BASELINE;
	}
    }
}

void
MathMLTableElement::SetupGroups()
{
  for (unsigned j = 0; j < nColumns; j++)
    {
      column[j].nAlignGroup = 0;
      
      for (unsigned i = 0; i < nRows; i++)
	{
	  cell[i][j].iGroup      = 0;
	  cell[i][j].nAlignGroup = 0;
	  cell[i][j].group       = NULL;
	  cell[i][j].aGroup      = NULL;

	  if (!cell[i][j].spanned && cell[i][j].mtd)
	    {
#if 0
	      // to be restored
	      MathMLTableCellElement::SetupGroups(cell[i][j].mtd->content.GetFirst(),
						  true, true,
						  cell[i][j]);
#endif

	      if (cell[i][j].nAlignGroup > column[j].nAlignGroup) 
		column[j].nAlignGroup = cell[i][j].nAlignGroup;

	      if (cell[i][j].nAlignGroup > 0)
		{
		  AlignmentGroup* aGroup = new AlignmentGroup[cell[i][j].nAlignGroup];
		  cell[i][j].aGroup = aGroup;
		}
	    }
	}
    }
}

void
MathMLTableElement::SetupGroupAlign(RenderingEnvironment& env)
{
  SmartPtr<Value> value = GetAttributeValue(ATTR_GROUPALIGN, env);
  SetupGroupAlignAux(value, 0, nRows);
}

void
MathMLTableElement::SetupGroupAlignAux(const SmartPtr<Value>& value,
				       unsigned rowStart,
				       unsigned n)
{
  assert(value);

  for (unsigned j = 0; j < nColumns; j++)
    {
      for (unsigned k = 0; k < column[j].nAlignGroup; k++)
	{
	  SmartPtr<Value> p = GetComponent(value, j, k);
	  assert(p);

	  GroupAlignId groupAlignment = ToGroupAlignId(p);

	  for (unsigned i = 0; i + 1 <= n; i++)
	    {
	      if (!cell[rowStart + i][j].spanned && k < cell[rowStart + i][j].nAlignGroup)
		cell[rowStart + i][j].aGroup[k].alignment = groupAlignment;
	    }
	}
    }
}

void
MathMLTableElement::SetupAlignMarks()
{
  for (unsigned i = 0; i < nRows; i++)
    for (unsigned j = 0; j < nColumns; j++)
      if (!cell[i][j].spanned && cell[i][j].mtd)
	{
#if 0
	  // to be restored
	  MathMLTableCellElement::SetupGroups(cell[i][j].mtd->content.GetFirst(),
					      true, false,
					      cell[i][j]);
#endif
	}
}

// finally, setup any attribute relative to the table itself and not
// any sub-element
void
MathMLTableElement::SetupTableAttributes(RenderingEnvironment& env)
{
  UnitValue unitValue;

  // align

  SmartPtr<Value> value = GetAttributeValue(ATTR_ALIGN, env);
  assert(value);

  SmartPtr<Value> p = GetComponent(value, 0);
  assert(p);

  switch (ToKeywordId(p))
    {
    case KW_TOP: align = TABLE_ALIGN_TOP; break;
    case KW_BOTTOM: align = TABLE_ALIGN_BOTTOM; break;
    case KW_CENTER: align = TABLE_ALIGN_CENTER; break;
    case KW_BASELINE: align = TABLE_ALIGN_BASELINE; break;
    case KW_AXIS:
      align = TABLE_ALIGN_AXIS;
      environmentAxis = env.GetAxis();
      break;
    default: assert(IMPOSSIBLE); break;
    }

  p = GetComponent(value, 1);
  if (IsEmpty(p))
    rowNumber = 0;
  else
    rowNumber = ToInteger(p);

  // rowlines

  value = GetAttributeValue(ATTR_ROWLINES, env);
  assert(value);

  for (unsigned i = 0; i < nRows; i++)
    {
      p = GetComponent(value, i);
      assert(p);
      row[i].lineType = ToLineId(p);
    }

  // columnlines

  value = GetAttributeValue(ATTR_COLUMNLINES, env);
  assert(value);

  for (unsigned j = 0; j < nColumns; j++)
    {
      p = GetComponent(value, j);
      assert(p);
      column[j].lineType = ToLineId(p);
    }

  // frame

  value = GetAttributeValue(ATTR_FRAME, env);
  assert(value);
  frame = ToLineId(value);

  // width

  value = GetAttributeValue(ATTR_WIDTH, env);
  assert(value);

  if (IsKeyword(value))
    {
      assert(ToKeywordId(value) == KW_AUTO);
      widthType = WIDTH_AUTO;
    } 
  else
    {
      assert(IsNumberUnit(value));
      unitValue = ToNumberUnit(value);
      if (unitValue.IsPercentage())
	{
	  widthType = WIDTH_PERCENTAGE;
	  scaleWidth = unitValue.GetValue();
	} 
      else
	{
	  widthType = WIDTH_FIXED;
	  fixedWidth = env.ToScaledPoints(unitValue);
	}
    }

  // framespacing

  value = GetAttributeValue(ATTR_FRAMESPACING, env);
  assert(value);

  p = Resolve(value, env, 0);
  assert(p && IsNumberUnit(p));

  unitValue = ToNumberUnit(p);
  if (unitValue.IsPercentage())
    {
      frameHorizontalSpacingType  = SPACING_PERCENTAGE;
      frameHorizontalScaleSpacing = unitValue.GetValue();
    } 
  else
    {
      frameHorizontalSpacingType  = SPACING_FIXED;
      frameHorizontalFixedSpacing = env.ToScaledPoints(unitValue);
    }

  p = Resolve(value, env, 1);
  assert(p && IsNumberUnit(p));

  unitValue = ToNumberUnit(p);
  if (unitValue.IsPercentage())
    {
      frameVerticalSpacingType  = SPACING_PERCENTAGE;
      frameVerticalScaleSpacing = unitValue.GetValue();
    } 
  else
    {
      frameVerticalSpacingType  = SPACING_FIXED;
      frameVerticalFixedSpacing = env.ToScaledPoints(unitValue);
    }

  if (frame == TABLE_LINE_NONE)
    {
      frameHorizontalSpacingType = frameVerticalSpacingType = SPACING_FIXED;
      frameHorizontalFixedSpacing = frameVerticalFixedSpacing = 0;
    }

  // equalrows

  equalRows = ToBoolean(GetAttributeValue(ATTR_EQUALROWS, env));

  // equalcolumns

  equalColumns = ToBoolean(GetAttributeValue(ATTR_EQUALCOLUMNS, env));

  // displaystyle

  displayStyle = ToBoolean(GetAttributeValue(ATTR_DISPLAYSTYLE, env));

  // side

  value = GetAttributeValue(ATTR_SIDE, env);
  assert(value);

  switch (ToKeywordId(value))
    {
    case KW_LEFT: side = TABLE_SIDE_LEFT; break;
    case KW_RIGHT: side = TABLE_SIDE_RIGHT; break;
    case KW_LEFTOVERLAP: side = TABLE_SIDE_LEFTOVERLAP; break;
    case KW_RIGHTOVERLAP: side = TABLE_SIDE_RIGHTOVERLAP; break;
    default: assert(IMPOSSIBLE); break;
    }

  // minlabelspacing

  value = GetAttributeValue(ATTR_MINLABELSPACING, env);
  assert(value && IsNumberUnit(value));

  unitValue = ToNumberUnit(value);
  if (unitValue.IsPercentage())
    {
      minLabelSpacingType  = SPACING_PERCENTAGE;
      minLabelScaleSpacing = unitValue.GetValue();
    } 
  else
    {
      minLabelSpacingType  = SPACING_FIXED;
      minLabelFixedSpacing = env.ToScaledPoints(unitValue);
    }
}

void
MathMLTableElement::ReleaseAuxStructures()
{
  if (row)
    {
      delete [] row;
      row = 0;
    }

  if (column)
    {
      delete [] column;
      column = 0;
    }

  if (cell)
    {
      for (unsigned i = 0; i < nRows; i++)
	{
	  for (unsigned j = 0; j < nColumns; j++)
	    delete [] cell[i][j].aGroup;
	  delete [] cell[i];
	}

      delete [] cell;
      cell = 0;
    }
}
