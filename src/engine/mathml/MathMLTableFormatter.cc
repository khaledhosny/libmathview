// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include <algorithm>
#include <cassert>

#include "Adapters.hh"
#include "MathMLTableFormatter.hh"
#include "MathMLValueConversion.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLTableFormatter::MathMLTableFormatter()
{ }

MathMLTableFormatter::~MathMLTableFormatter()
{ }

#include "BoundingBoxAux.hh"
#include "scaledAux.hh"

void
MathMLTableFormatter::Column::setWidthSpec(const FormattingContext& ctxt, const Length& spec)
{
  if (spec.type == Length::PERCENTAGE_UNIT)
    setWidthSpec(spec.value / 100.0);
  else
    setWidthSpec(ctxt.MGD()->evaluate(ctxt, spec, scaled::zero()));
}

void
MathMLTableFormatter::Row::setHeightSpec(const FormattingContext& ctxt, const Length& spec)
{
  if (spec.type == Length::PERCENTAGE_UNIT)
    setHeightSpec(spec.value / 100.0);
  else
    setHeightSpec(ctxt.MGD()->evaluate(ctxt, spec, scaled::zero()));
}

void
MathMLTableFormatter::init(const FormattingContext& ctxt,
			   unsigned nR,
			   unsigned nC,
			   const std::vector<SmartPtr<MathMLTableCellElement> >& cell,
			   const std::vector<SmartPtr<MathMLTableCellElement> >& label,
			   const SmartPtr<Value>& columnWidthV,
			   const SmartPtr<Value>& rowSpacingV,
			   const SmartPtr<Value>& columnSpacingV,
			   const SmartPtr<Value>& frameV,
			   const SmartPtr<Value>& frameSpacingV,
			   const SmartPtr<Value>& equalRowsV,
			   const SmartPtr<Value>& equalColumnsV,
			   const SmartPtr<Value>& sideV,
			   const SmartPtr<Value>& minLabelSpacingV,
			   const SmartPtr<Value>& alignV)
{
  axis = ctxt.MGD()->axis(ctxt);

  nRows = nR;
  nColumns = nC;

  tableAlign = ToTokenId(GetComponent(alignV, 0));
  if (IsEmpty(GetComponent(alignV, 1)))
    tableAlignRow = 0;
  else
    tableAlignRow = ToInteger(GetComponent(alignV, 1));

  const TokenId frame = ToTokenId(frameV);
  const TokenId side = ToTokenId(sideV);
  const bool hasFrame = frame == T_SOLID || frame == T_DASHED;
  const bool hasLabels = std::find_if(label.begin(), label.end(), NotNullPredicate<MathMLTableCellElement>()) != label.end();
  
  const unsigned nGridRows = (hasFrame ? 2 : 0) + ((nRows > 0) ? (nRows * 2 - 1) : 0);
  const unsigned nGridColumns = (hasFrame ? 2 : 0) + (hasLabels ? 4 : 0) + ((nColumns > 0) ? (nColumns * 2 - 1) : 0);
  const unsigned contentColumnOffset = (hasFrame ? 1 : 0) + (hasLabels ? 2 : 0);
  const unsigned contentRowOffset = (hasFrame ? 1 : 0);
  const unsigned leftLabelOffset = (hasFrame ? 1 : 0);
  const unsigned rightLabelOffset = (hasFrame ? 1 : 0) + nColumns * 2 + 2;
  const unsigned labelOffset = (side == T_LEFT || side == T_LEFTOVERLAP) ? leftLabelOffset : rightLabelOffset;

  equalRows = ToBoolean(equalRowsV);
  equalColumns = ToBoolean(equalColumnsV);

  assert(nGridRows >= 0);
  assert(nGridColumns >= 0);

#if 0
  std::cerr << "CI SIAMO: " << nRows << "x" << nColumns << std::endl
	    << "grid: " << nGridRows << "x" << nGridColumns << std::endl
	    << "frame? " << hasFrame << " labels? " << hasLabels << std::endl;
#endif

  std::vector<Row>(nGridRows).swap(rows);
  std::vector<Column>(nGridColumns).swap(columns);
  std::vector<Cell>(nGridRows * nGridColumns).swap(cells);

  //std::cerr << "HAS FRAME?" << hasFrame << std::endl;
  if (hasFrame)
    {
      const Length hFrameSpacing = resolveLength(ctxt, frameSpacingV, 0);
      const Length vFrameSpacing = resolveLength(ctxt, frameSpacingV, 1);
      rows.front().setHeightSpec(ctxt, vFrameSpacing);
      rows.back().setHeightSpec(ctxt, vFrameSpacing);
      columns.front().setWidthSpec(ctxt, hFrameSpacing);
      columns.back().setWidthSpec(ctxt, hFrameSpacing);
    }

  //std::cerr << "HAS LABELS?" << hasLabels << std::endl;
  if (hasLabels)
    {
      const Length minLabelSpacing = resolveLength(ctxt, minLabelSpacingV);
      columns[leftLabelOffset].setWidthSpec(Column::FIT);
      columns[rightLabelOffset].setWidthSpec(Column::FIT);
      if (side == T_LEFT || side == T_LEFTOVERLAP)
	{
	  columns[leftLabelOffset + 1].setWidthSpec(ctxt, minLabelSpacing);
	  columns[rightLabelOffset - 1].setWidthSpec(scaled::zero());
	}
      else
	{
	  columns[leftLabelOffset + 1].setWidthSpec(scaled::zero());
	  columns[rightLabelOffset - 1].setWidthSpec(ctxt, minLabelSpacing);
	}
    }

  //std::cerr << "SETUP COLUMNS" << std::endl;
  for (unsigned j = 0; j < nColumns; j++)
    {
      const unsigned jj = contentColumnOffset + j * 2;
      //std::cerr << "setup column " << jj << std::endl;
      if (equalColumns)
	columns[jj].setWidthSpec(Column::AUTO);
      else
	{
	  const SmartPtr<Value> specV = GetComponent(columnWidthV, j);
	  if (isTokenId(specV, T_AUTO))
	    columns[jj].setWidthSpec(Column::AUTO);
	  else if (isTokenId(specV, T_FIT))
	    columns[jj].setWidthSpec(Column::FIT);
	  else
	    columns[jj].setWidthSpec(ctxt, resolveLength(ctxt, specV));
	}
      columns[jj].setContentColumn();

      if (j + 1 < nColumns)
	columns[jj + 1].setWidthSpec(ctxt, resolveLength(ctxt, columnSpacingV, j));
    }

  //std::cerr << "SETUP ROWS" << std::endl;
  for (unsigned i = 0; i < nRows; i++)
    {
      const unsigned ii = contentRowOffset + i * 2;

      if (hasLabels)
	cells[ii * nGridColumns + labelOffset].setContent(label[i]);

      rows[ii].setHeightSpec(Row::AUTO);
      for (unsigned j = 0; j < nColumns; j++)
	{
	  const unsigned jj = contentColumnOffset + j * 2;
	  cells[ii * nGridColumns + jj].setContent(cell[i * nColumns + j]);
	}
      rows[ii].setContentRow();

      if (i + 1 < nRows)
	rows[ii + 1].setHeightSpec(ctxt, resolveLength(ctxt, rowSpacingV, i));
    }
}

void
MathMLTableFormatter::formatCells(const FormattingContext& ctxt,
				  const scaled& availableWidth,
				  const SmartPtr<Value>& width) const
{
}

AreaRef
MathMLTableFormatter::formatLines(const FormattingContext& ctxt,
				  const SmartPtr<Value>& frameV,
				  const SmartPtr<Value>& rowLinesV,
				  const SmartPtr<Value>& columnLinesV) const
{
  const TokenId frame = ToTokenId(frameV);
  const unsigned nGridRows = rows.size();
  const unsigned nGridColumns = columns.size();
  const scaled defaultLineThickness = ctxt.MGD()->defaultLineThickness(ctxt);
  const RGBColor color = ctxt.getColor();

  std::vector<BoxedLayoutArea::XYArea> content;
  for (unsigned ii = 0; ii < nGridRows; ii++)
    if (rows[ii].isContentRow())
      for (unsigned jj = 0; jj < nGridColumns; jj++)
	if (const Cell& cell = getCell(ii, jj))
	  {
	    const SmartPtr<MathMLTableCellElement> el = cell.getContent();
	    assert(el);
	    const unsigned i = el->getRowIndex();
	    const unsigned j = el->getColumnIndex();
	    //std::cerr << "LINES: FOUND ELEMENT at " << i << "," << j << std::endl;
	    if (i + el->getRowSpan() < nRows
		&& ToTokenId(GetComponent(rowLinesV, i + el->getRowSpan() - 1)) != T_NONE)
	      {
		const scaled dx0 =
		  (j == 0) ?
		  ((frame != T_NONE) ? columns[jj - 1].getLeftDisplacement()
		   : columns[jj].getLeftDisplacement())
		  : columns[jj - 1].getCenterDisplacement();
		const scaled dx1 =
		  (j + el->getColumnSpan() == nColumns) ?
		  ((frame != T_NONE) ? columns[jj + cell.getColumnSpan()].getRightDisplacement()
		   : columns[jj + cell.getColumnSpan() - 1].getRightDisplacement())
		  : columns[jj + cell.getColumnSpan()].getCenterDisplacement();
		const scaled dy =
		  rows[ii + cell.getRowSpan()].getCenterDisplacement();
		BoxedLayoutArea::XYArea area(dx0, dy,
					     ctxt.MGD()->getFactory()->fixedHorizontalLine(defaultLineThickness,
											   dx1 - dx0, color));
		//std::cerr << "draw x line " << dx0 << "," << dy << " to " << dx1 << std::endl;
		content.push_back(area);
	      }

	    if (j + el->getColumnSpan() < nColumns
		&& ToTokenId(GetComponent(columnLinesV, j + el->getColumnSpan() - 1)) != T_NONE)
	      {
		const scaled dy0 =
		  (i == 0) ?
		  ((frame != T_NONE) ? rows[ii - 1].getTopDisplacement()
		   : rows[ii].getTopDisplacement())
		  : rows[ii - 1].getCenterDisplacement();
		const scaled dy1 =
		  (i + el->getRowSpan() == nRows) ?
		  ((frame != T_NONE) ? rows[ii + cell.getRowSpan()].getBottomDisplacement()
		   : rows[ii + cell.getRowSpan() - 1].getBottomDisplacement())
		  : rows[ii + cell.getRowSpan()].getCenterDisplacement();
		const scaled dx =
		  columns[jj + cell.getColumnSpan()].getCenterDisplacement();
		BoxedLayoutArea::XYArea area(dx, dy0,
					     ctxt.MGD()->getFactory()->fixedVerticalLine(defaultLineThickness,
											 scaled::zero(),
											 dy0 - dy1, color));
		//std::cerr << "draw y line " << dx << "," << dy0 << " to " << dy1 << std::endl;
		content.push_back(area);
	      }
	  }

  if (frame != T_NONE)
    {			
      //std::cerr << "HAS FRAME" << std::endl;
      const scaled left = columns.front().getLeftDisplacement();
      const scaled right = columns.back().getRightDisplacement();
      const scaled top = rows.front().getTopDisplacement();
      const scaled bottom = rows.back().getBottomDisplacement();

      const AreaRef hline = ctxt.MGD()->getFactory()->fixedHorizontalLine(defaultLineThickness, right - left + defaultLineThickness, color);
      const AreaRef vline = ctxt.MGD()->getFactory()->fixedVerticalLine(defaultLineThickness, scaled::zero(), top - bottom, color);

      content.push_back(BoxedLayoutArea::XYArea(scaled::zero(), top, hline));
      content.push_back(BoxedLayoutArea::XYArea(scaled::zero(), bottom, hline));
      content.push_back(BoxedLayoutArea::XYArea(scaled::zero(), top, vline));
      content.push_back(BoxedLayoutArea::XYArea(right, top, vline));
    }

  return content.empty() ? 0 : ctxt.MGD()->getFactory()->boxedLayout(getBoundingBox(), content);
}

scaled
MathMLTableFormatter::computeTableWidth(const scaled& minimumTableWidth)
{
  // TODO
  return minimumTableWidth;
}

BoundingBox
MathMLTableFormatter::assignTableWidth(const scaled& minimumTableWidth)
{
  const scaled tableWidth = computeTableWidth(minimumTableWidth);
  setWidth(tableWidth);

  if (equalColumns)
    assignTableWidthT(width);
  else
    assignTableWidthF(width);

  // TODO: assignment propagation

  initTempHeightDepth();
  const scaled tableHeightDepth = equalRows ? computeTableHeightDepthT() : computeTableHeightDepthF();

  if (tableAlignRow == 0)
    alignTable(tableHeightDepth, axis, tableAlign);
  else
    {
      const unsigned contentRowOffset = rows[0].isContentRow() ? 0 : 1;
      const unsigned gridRow = contentRowOffset + 2 * ((tableAlignRow < 0) ? (nRows + tableAlignRow) : (tableAlignRow - 1));
      alignTable(tableHeightDepth, axis, tableAlign, gridRow);
    }
  setDisplacements();
  setCellPosition();

  return getBoundingBox();
}

BoundingBox
MathMLTableFormatter::format(std::vector<BoxedLayoutArea::XYArea>& content)
{
  initTempWidths();
  const BoundingBox tableBox = assignTableWidth(computeMinimumTableWidth());

  content.clear();
  for (std::vector<Cell>::const_iterator p = cells.begin();
       p != cells.end();
       p++)
    if (!p->isNull())
      {
	scaled dx;
	scaled dy;
	p->getDisplacement(dx, dy);
	content.push_back(BoxedLayoutArea::XYArea(dx, dy, p->getArea()));
      }

  return tableBox;
}

scaled
MathMLTableFormatter::getColumnContentWidth(unsigned j) const
{
  scaled maxWidth = 0;
  for (unsigned i = 0; i < rows.size(); i++)
    {
      if (rows[i].isContentRow())
	if (const Cell& cell = getCell(i, j))
	  if (cell.getColumnSpan() == 1)
	    maxWidth = std::max(maxWidth, cell.getBoundingBox().width);
    }
  //std::cerr << "content width[" << j << "] = " << maxWidth << std::endl;
  return maxWidth;
}

void
MathMLTableFormatter::initTempWidths()
{
  for (unsigned j = 0; j < columns.size(); j++)
    {
      if (columns[j].isContentColumn() && columns[j].getSpec() != Column::FIX)
	{
	  const scaled contentWidth = getColumnContentWidth(j);
	  columns[j].setContentWidth(contentWidth);
	  columns[j].setTempWidth(contentWidth);
	}
      else if (columns[j].getSpec() == Column::FIX)
	columns[j].setTempWidth(columns[j].getFixWidth());
      else if (columns[j].getSpec() == Column::SCALE && !columns[j].isContentColumn())
	columns[j].setTempWidth(0);
    }

  for (unsigned j = 0; j < columns.size(); j++)
    if (columns[j].isContentColumn())
      for (unsigned i = 0; i < rows.size(); i++)
	if (rows[i].isContentRow())
	  if (const Cell& cell = getCell(i, j))
	    if (cell.getColumnSpan() > 1)
	      {
		//std::cerr << "CELL " << i << "," << j << " " << cell.getColumnSpan() << " " << cell.getBoundingBox() << std::endl;
		const scaled cellWidth = cell.getBoundingBox().width;
		scaled spannedTempWidth = 0;
		int n = 0;
		for (unsigned z = j; z <= j + cell.getColumnSpan() - 1; z++)
		  {
		    spannedTempWidth += columns[z].getTempWidth();
		    if (columns[z].isContentColumn() && columns[j].getSpec() != Column::FIX)
		      n++;
		  }
		if (cellWidth > spannedTempWidth)
		  for (unsigned z = j; z <= j + cell.getColumnSpan() - 1; z++)
		    if (columns[z].isContentColumn() && columns[j].getSpec() != Column::FIX)
		      columns[z].setTempWidth(columns[z].getTempWidth() + (cellWidth - spannedTempWidth) / n);
	      }
}

scaled
MathMLTableFormatter::computeMinimumTableWidthT()
{
  numCol = 0;
  sumFix = 0;
  sumCont = 0;
  sumScale = 0;

  scaled max = 0;                                                                                            
  for (unsigned j = 0; j < columns.size(); j++)
    {
      if (columns[j].isContentColumn())
	{
	  numCol++;
	  max = std::max(max, columns[j].getTempWidth());
	}
      else if (columns[j].getSpec() == Column::FIX)
	sumFix += columns[j].getFixWidth();
      else if (columns[j].getSpec() == Column::SCALE)
	sumScale += columns[j].getScaleWidth();
    }	
    
  return std::max(numCol * max + sumFix, ((numCol * max) + sumFix) / (1 - sumScale));
}

scaled
MathMLTableFormatter::computeMinimumTableWidth()
{
  const scaled minimumTableWidth = equalColumns ? computeMinimumTableWidthT() : computeMinimumTableWidthF();
  // TODO: gestire widthSpec
  return minimumTableWidth;
}

void
MathMLTableFormatter::assignTableWidthT(const scaled& tableWidth)
{
  const scaled assignedWidth = sumFix + tableWidth * sumScale;
  const scaled availWidth = std::max(scaled::zero(), tableWidth - assignedWidth);
	
  for (unsigned j = 0; j < columns.size(); j++)
    if (columns[j].isContentColumn())
      columns[j].setWidth(availWidth / numCol);
    else if (columns[j].getSpec() == Column::FIX)
      columns[j].setWidth(columns[j].getFixWidth());
    else if (columns[j].getSpec() == Column::SCALE)
      columns[j].setWidth(tableWidth * columns[j].getScaleWidth());
}

scaled
MathMLTableFormatter::computeMinimumTableWidthF()
{
  numCol = 0;
  sumCont = 0;
  sumFix = 0;
  sumScale = 0;

  scaled max = 0;
  scaled tempWidth = 0;
	
  for (unsigned j = 0; j < columns.size(); j++)
    if (columns[j].isContentColumn() && columns[j].getSpec() == Column::SCALE)
      max = std::max(max, columns[j].getTempWidth() / columns[j].getScaleWidth());

  for (unsigned j = 0; j < columns.size(); j++)
    if (columns[j].isContentColumn()
	&& columns[j].getSpec() != Column::FIX
	&& columns[j].getSpec() != Column::SCALE)
      {
	sumCont += columns[j].getTempWidth();
	numCol++;
      }
    else if (columns[j].getSpec() == Column::FIX)
      sumFix += columns[j].getFixWidth();
    else if (columns[j].getSpec() == Column::SCALE)
      sumScale += columns[j].getScaleWidth();

  return std::max(max, std::max(sumCont + sumFix, (sumCont + sumFix) / (1 - sumScale)));
}

void
MathMLTableFormatter::assignTableWidthF(const scaled& tableWidth)
{
  const scaled assignedWidth = sumFix + tableWidth * sumScale;
  const scaled extraWidth = std::max(scaled::zero(), tableWidth - assignedWidth - sumCont);

#if 0
  std::cerr << "initWidthsF tableWidth = " << tableWidth << std::endl
	  << "sumScale = " << sumScale << std::endl
	  << "assignedWidth = " << assignedWidth << std::endl
	  << "extraWidth = " << extraWidth << std::endl;
#endif

  for (unsigned j = 0; j < columns.size(); j++)
    if ((columns[j].isContentColumn()
	 && columns[j].getSpec() != Column::FIX
	 && columns[j].getSpec() != Column::SCALE))
      columns[j].setWidth(columns[j].getTempWidth() + (extraWidth / numCol));
    else if (columns[j].getSpec() == Column::FIX)
      columns[j].setWidth(columns[j].getFixWidth());
    else if (columns[j].getSpec() == Column::SCALE)
      columns[j].setWidth(tableWidth * columns[j].getScaleWidth());
}

void
MathMLTableFormatter::initTempHeightDepth()
{
  for (unsigned i = 0; i < rows.size(); i++)
    {
      if (rows[i].getSpec() == Row::FIX)
	{
	  rows[i].setTempHeight(rows[i].getFixHeight());
	  rows[i].setTempDepth(0);
	}
      else if (rows[i].getSpec() == Row::SCALE)
	{
	  rows[i].setTempHeight(0);
	  rows[i].setTempDepth(0);
	}
      else if (rows[i].isContentRow())
	{
	  scaled maxH = 0;
	  scaled maxD = 0;
	  for (unsigned j = 0; j < columns.size(); j++)
	    if (const Cell& cell = getCell(i, j))
	      if (cell.getRowSpan() == 1)
		switch (cell.getRowAlign())
		  {
		  case T_BASELINE:
		    {
		      const BoundingBox box = cell.getBoundingBox();
		      maxH = std::max(maxH, box.height);
		      maxD = std::max(maxD, box.depth);
		    }
		    break;
		  case T_AXIS:
		    {
		      const BoundingBox box = cell.getBoundingBox();
		      maxH = std::max(maxH, box.height - axis);
		      maxD = std::max(maxD, box.depth + axis);
		    }
		    break;
		  default:
		    break;
		  }
	  rows[i].setTempHeight(maxH);
	  rows[i].setTempDepth(maxD);
	}
    }

  for (unsigned i = 0; i < rows.size(); i++)
    if (rows[i].isContentRow())
      for (unsigned j = 0; j < columns.size(); j++)
	if (columns[j].isContentColumn())
	  if (const Cell& cell = getCell(i, j))
	    if (cell.getRowSpan() == 1 && cell.getRowAlign() != T_BASELINE && cell.getRowAlign() != T_AXIS)
	      {
		const BoundingBox box = cell.getBoundingBox();
		if ((rows[i].getTempHeight() + rows[i].getTempDepth()) < box.verticalExtent())
		  rows[i].setTempDepth(box.verticalExtent() - rows[i].getTempHeight());
	      }
	
  for (unsigned i = 0; i < rows.size(); i++)
    if (rows[i].isContentRow())
      for (unsigned j = 0; j < columns.size(); j++)
	if (columns[j].isContentColumn())
	  if (const Cell& cell = getCell(i, j))
	    if (cell.getRowSpan() > 1)
	      {
		const scaled cellHeightDepth = cell.getBoundingBox().verticalExtent();
		scaled spannedTempHeightDepth = 0;
		int n = 0;
		for (unsigned z = i; z <= i + cell.getRowSpan() - 1; z++)
		  {
		    spannedTempHeightDepth += rows[z].getTempHeight() + rows[z].getTempDepth();
		    if (rows[z].isContentRow()) n++;
		  }
#if 0
		std::cerr << "CELL " << i << "," << j
			  << " cellHeightDepth = " << cellHeightDepth
			  << " spannedTempHeightDepth = " << spannedTempHeightDepth << std::endl;
#endif
		if (cellHeightDepth > spannedTempHeightDepth)
		  {
		    for (unsigned z = i; z <= i + cell.getRowSpan() - 1; z++)
		      if (rows[z].isContentRow())
			rows[z].setTempDepth(rows[z].getTempDepth() + (cellHeightDepth - spannedTempHeightDepth) / n);
		  }
	      }
}

scaled
MathMLTableFormatter::computeTableHeightDepthT()
{
  int numRows = 0;
  float sumScale = 0.0f;
  scaled sumFixHD = 0;
  scaled sumContHD = 0;
  scaled max = 0;

  for (unsigned i = 0; i < rows.size(); i++)
    {
      if (rows[i].isContentRow())
	{
	  numRows++;
	  max = std::max(max, rows[i].getTempHeight() + rows[i].getTempDepth());
	  sumContHD += rows[i].getTempHeight() + rows[i].getTempDepth();
	}
      else if (rows[i].getSpec() == Row::FIX)
	sumFixHD += rows[i].getFixHeight();
      else if (rows[i].getSpec() == Row::SCALE)
	sumScale += rows[i].getScaleHeight();
    }

  const scaled tableHeightDepth = std::max(numRows * max + sumFixHD, ((numRows * max) + sumFixHD) / (1 - sumScale));
  const scaled assignedHeightDepth = sumFixHD + tableHeightDepth * sumScale;
  const scaled availHeightDepth = std::max(scaled::zero(), tableHeightDepth - assignedHeightDepth);
	
  for (unsigned i = 0; i < rows.size(); i++)
    if (rows[i].isContentRow())
      {
	rows[i].setHeight(rows[i].getTempHeight());	
	rows[i].setDepth(availHeightDepth / numRows - rows[i].getHeight());
	//std::cerr << "for row" << i << " HD = " << rows[i].getHeight() + rows[i].getDepth() << std::endl;
      }
    else if (rows[i].getSpec() == Row::FIX)
      {
	rows[i].setHeight(rows[i].getFixHeight());
	rows[i].setDepth(0);
      }
    else if (rows[i].getSpec() == Row::SCALE)
      {
	rows[i].setHeight(tableHeightDepth * rows[i].getScaleHeight());
	rows[i].setDepth(0);
      }

  return tableHeightDepth;
}

scaled
MathMLTableFormatter::computeTableHeightDepthF()
{
  scaled sumContFix = 0;
  float sumScale = 0;

  for (unsigned i = 0; i < rows.size(); i++)
    if (rows[i].isContentRow() || rows[i].getSpec() == Row::FIX)
      {
	sumContFix += rows[i].getTempHeight() + rows[i].getTempDepth();
      }
    else if (rows[i].getSpec() == Row::SCALE)
      sumScale += rows[i].getScaleHeight();         

  const scaled tableHeightDepth = std::max(sumContFix, sumContFix / (1 - sumScale));

  for (unsigned i = 0; i < rows.size(); i++)
    if (rows[i].isContentRow())
      {
	rows[i].setHeight(rows[i].getTempHeight());
	rows[i].setDepth(rows[i].getTempDepth());
      }
    else if (rows[i].getSpec() == Row::FIX)
      {
	rows[i].setHeight(rows[i].getFixHeight());
	rows[i].setDepth(0);
      }
    else if (rows[i].getSpec() == Row::SCALE)
      {
	rows[i].setHeight(tableHeightDepth * rows[i].getScaleHeight());
	rows[i].setDepth(0);
      }

  return tableHeightDepth;
}

void
MathMLTableFormatter::alignTable(const scaled& tableHeightDepth, const scaled& axis, TokenId align)
{
  switch (align)
    {
    case T_TOP:
      setHeight(0);
      break;
    case  T_BOTTOM:
      setHeight(tableHeightDepth);
      break;
    case T_AXIS:
      setHeight((tableHeightDepth / 2) + axis);
      break;
    case T_CENTER:
    case T_BASELINE:
      setHeight(tableHeightDepth / 2);
      break;
    default:
      assert(false);
    }

  setDepth(tableHeightDepth - getHeight());
}

void
MathMLTableFormatter::alignTable(const scaled& tableHeightDepth, const scaled& axis, TokenId align, unsigned rowNum)
{
  scaled temp = 0;

  for (unsigned i = 0; i < rowNum; i++)
    temp += rows[i].getVerticalExtent();

  switch (align)
    {
    case T_TOP:
      break;
    case T_BOTTOM:
      temp += rows[rowNum].getVerticalExtent();
      break;
    case T_AXIS:
      temp += rows[rowNum].getHeight() - axis;
      break;
    case T_CENTER:
      temp += rows[rowNum].getVerticalExtent() / 2;
      break;
    case T_BASELINE:
      temp += rows[rowNum].getHeight();
      break;
    default:
      assert(false);
    }
  setHeight(temp);
  setDepth(tableHeightDepth - temp);
}

BoundingBox
MathMLTableFormatter::getCellBoundingBox(unsigned i, unsigned j, unsigned rowSpan, unsigned columnSpan) const
{
  BoundingBox box(columns[j].getWidth(), rows[i].getHeight(), rows[i].getDepth());

  for (unsigned k = i + 1; k < i + rowSpan; k++)
    box.depth += rows[k].getVerticalExtent();

  for (unsigned k = j + 1; k < j + columnSpan; k++)
    box.width += columns[k].getWidth();

  return box;
}

void
MathMLTableFormatter::setDisplacements()
{
  scaled v = getHeight();
  for (unsigned i = 0; i < rows.size(); i++)
    {
      rows[i].setDisplacement(v - rows[i].getHeight());
      v -= rows[i].getVerticalExtent();
      //std::cerr << "ROW[" << i << "].displacement = " << rows[i].getDisplacement() << std::endl;
    }

  scaled h = scaled::zero();
  for (unsigned j = 0; j < columns.size(); j++)
    {
      columns[j].setDisplacement(h);
      h += columns[j].getWidth();
      //std::cerr << "COL[" << j << "].displacement = " << columns[j].getDisplacement() << std::endl;
    }
}

void
MathMLTableFormatter::setCellPosition()
{
  for (unsigned i = 0; i < rows.size(); i++)
    if (rows[i].isContentRow())
      for (unsigned j = 0; j < columns.size(); j++)
	if (columns[j].isContentColumn())
	  {
	    if (const Cell& cell = getCell(i, j))
	      {
		scaled dx = scaled::zero();
		scaled dy = scaled::zero();

		const BoundingBox box = cell.getBoundingBox();
		const BoundingBox cellBox = getCellBoundingBox(i, j, cell.getRowSpan(), cell.getColumnSpan());

		//std::cerr << "CELL BOX = " << cellBox << std::endl << " CONTENT BOX = " << box << std::endl;

		switch (cell.getColumnAlign())
		  {
		  case T_LEFT:
		    dx = scaled::zero();
		    break;
		  case T_RIGHT:
		    dx = cellBox.width - box.width;
		    break;
		  case T_CENTER:
		    dx = (cellBox.width - box.width) / 2;
		    break;
		  default:
		    assert(false);
		  }

		switch (cell.getRowAlign())
		  {
		  case T_BASELINE:
		    dy = scaled::zero();
		    break;
		  case T_TOP:
		    dy = cellBox.height - box.height;
		    break;
		  case T_BOTTOM:
		    dy = box.depth - cellBox.depth;
		    break;
		  case T_CENTER:
		    dy = (cellBox.height - cellBox.depth - box.height + box.depth) / 2;
		    break;
		  case T_AXIS:
		    dy = -axis;
		    break;
		  default:
		    assert(false);
		  }

		//std::cerr << "setting displacement for (" << i << "," << j << ") = " << dx << "," << dy << std::endl;
		cell.setDisplacement(columns[j].getDisplacement() + dx, rows[i].getDisplacement() + dy);
	      }
	  }
}

const MathMLTableFormatter::Cell&
MathMLTableFormatter::getCell(unsigned i, unsigned j) const
{
  assert(i < rows.size());
  assert(j < columns.size());
  return cells[i * columns.size() + j];
}
