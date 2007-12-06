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

#include <vector>

#include "defs.h"
#include "ValueConversion.hh"
#include "MathMLTableElement.hh"
#include "MathMLTableCellElement.hh"
#include "MathMLLabeledTableRowElement.hh"

#if 0
void
MathMLTableElement::Setup(RenderingEnvironment& env)
{
  //printf("redoing table setup %p %d %d\n", this, (dirtyAttribute() || dirtyAttributeP()), (row ? sp2ipx(row[0].GetHeight()) : -1));
  if (dirtyAttribute() || dirtyAttributeP())
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
      resetDirtyAttribute();
      setDirtyLayout(); // DIRTY PATCH!
    }
  //printf("done table setup %p %d %d\n", this, (dirtyAttribute() || dirtyAttributeP()), (row ? sp2ipx(row[0].GetHeight()) : -1));
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
class TempRow
{
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
    return std::max(first, content.size());
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

  SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Table, columnwidth);

  for (i = 0; i < nColumns; i++)
    {
      SmartPtr<Value> v = Resolve(value, env, i);
      assert(v);

      if (IsTokenId(v))
	switch (ToTokenId(v))
	  {
	  case T_AUTO: column[i].widthType = COLUMN_WIDTH_AUTO; break;
	  case T_FIT: column[i].widthType = COLUMN_WIDTH_FIT; break;
	  default: assert(IMPOSSIBLE); break;
	  }
      else
	{
	  assert(IsLength(v));
	
	  Length unitValue = ToLength(v);

	  if (unitValue.type == Length::PERCENTAGE_UNIT)
	    {
	      column[i].widthType  = COLUMN_WIDTH_PERCENTAGE;
	      column[i].scaleWidth = unitValue.value;
	    } 
	  else
	    {
	      column[i].widthType  = COLUMN_WIDTH_FIXED;
	      column[i].fixedWidth = env.ToScaledPoints(unitValue);
	    }
	}
    }

  value = GET_ATTRIBUTE_VALUE(Table, columnspacing);

  for (i = 0; i < nColumns; i++)
    {
      SmartPtr<Value> v = Resolve(value, env, i);
      assert(IsLength(v));

      Length unitValue = ToLength(v);

      if (unitValue.type == Length::PERCENTAGE_UNIT)
	{
	  column[i].spacingType  = SPACING_PERCENTAGE;
	  column[i].scaleSpacing = unitValue.value;
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
  SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Table, alignmentscope);
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
  SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Table, columnalign);
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

      TokenId columnAlign = ToTokenId(p);
      
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
	    if (cell[rowStart + i][j - j0].columnAlign == T__NOTVALID ||
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

  SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Table, rowspacing);
  assert(value);

  for (i = 0; i < nRows; i++)
    {
      SmartPtr<Value> p = GetComponent(value, i);
      assert(p && IsLength(p));

      Length unitValue = ToLength(p);

      if (unitValue.type == Length::PERCENTAGE_UNIT)
	{
	  row[i].spacingType  = SPACING_PERCENTAGE;
	  row[i].scaleSpacing = unitValue.value;
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
  SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Table, rowalign);
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

  TokenId rowAlign = ToTokenId(value);

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
	  if (side == T_LEFT || side == T_LEFTOVERLAP)
	    rowLabel[i].columnAlign = T_LEFT;
	  else
	    rowLabel[i].columnAlign = T_RIGHT;
	  rowLabel[i].rowAlign = T_BASELINE;
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
  SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Table, groupalign);
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

	  TokenId groupAlignment = ToTokenId(p);

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
  Length unitValue;

  // align

  SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Table, align);
  assert(value);

  SmartPtr<Value> p = GetComponent(value, 0);
  assert(p);

  align = ToTokenId(p);
  environmentAxis = env.GetAxis();

  p = GetComponent(value, 1);
  if (IsEmpty(p))
    rowNumber = 0;
  else
    rowNumber = ToInteger(p);

  // rowlines

  value = GET_ATTRIBUTE_VALUE(Table, rowlines);
  assert(value);

  for (unsigned i = 0; i < nRows; i++)
    {
      p = GetComponent(value, i);
      assert(p);
      row[i].lineType = ToTokenId(p);
    }

  // columnlines

  value = GET_ATTRIBUTE_VALUE(Table, columnlines);
  assert(value);

  for (unsigned j = 0; j < nColumns; j++)
    {
      p = GetComponent(value, j);
      assert(p);
      column[j].lineType = ToTokenId(p);
    }

  // frame

  value = GET_ATTRIBUTE_VALUE(Table, frame);
  assert(value);
  frame = ToTokenId(value);

  // width

  value = GET_ATTRIBUTE_VALUE(Table, width);
  assert(value);

  if (IsTokenId(value))
    {
      assert(ToTokenId(value) == T_AUTO);
      widthType = WIDTH_AUTO;
    } 
  else
    {
      assert(IsLength(value));
      unitValue = ToLength(value);
      if (unitValue.type == Length::PERCENTAGE_UNIT)
	{
	  widthType = WIDTH_PERCENTAGE;
	  scaleWidth = unitValue.value;
	} 
      else
	{
	  widthType = WIDTH_FIXED;
	  fixedWidth = env.ToScaledPoints(unitValue);
	}
    }

  // framespacing

  value = GET_ATTRIBUTE_VALUE(Table, framespacing);
  assert(value);

  p = Resolve(value, env, 0);
  assert(p && IsLength(p));

  unitValue = ToLength(p);
  if (unitValue.type == Length::PERCENTAGE_UNIT)
    {
      frameHorizontalSpacingType  = SPACING_PERCENTAGE;
      frameHorizontalScaleSpacing = unitValue.value;
    } 
  else
    {
      frameHorizontalSpacingType  = SPACING_FIXED;
      frameHorizontalFixedSpacing = env.ToScaledPoints(unitValue);
    }

  p = Resolve(value, env, 1);
  assert(p && IsLength(p));

  unitValue = ToLength(p);
  if (unitValue.type == Length::PERCENTAGE_UNIT)
    {
      frameVerticalSpacingType  = SPACING_PERCENTAGE;
      frameVerticalScaleSpacing = unitValue.value;
    } 
  else
    {
      frameVerticalSpacingType  = SPACING_FIXED;
      frameVerticalFixedSpacing = env.ToScaledPoints(unitValue);
    }

  if (frame == T_NONE)
    {
      frameHorizontalSpacingType = frameVerticalSpacingType = SPACING_FIXED;
      frameHorizontalFixedSpacing = frameVerticalFixedSpacing = 0;
    }

  // equalrows

  equalRows = ToBoolean(GET_ATTRIBUTE_VALUE(Table, equalrows));

  // equalcolumns

  equalColumns = ToBoolean(GET_ATTRIBUTE_VALUE(Table, equalcolumns));

  // displaystyle

  displayStyle = ToBoolean(GET_ATTRIBUTE_VALUE(Table, displaystyle));

  // side

  value = GET_ATTRIBUTE_VALUE(Table, side);
  assert(value);
  side = ToTokenId(value);

  // minlabelspacing

  value = GET_ATTRIBUTE_VALUE(Table, minlabelspacing);
  assert(value && IsLength(value));

  unitValue = ToLength(value);
  if (unitValue.type == Length::PERCENTAGE_UNIT)
    {
      minLabelSpacingType  = SPACING_PERCENTAGE;
      minLabelScaleSpacing = unitValue.value;
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
#endif
