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

#ifndef __MathMLTableFormatter_hh__
#define __MathMLTableFormatter_hh__

#include <vector>

#include "token.hh"
#include "Object.hh"
#include "SmartPtr.hh"
#include "BoxedLayoutArea.hh"
#include "MathMLTableCellElement.hh"

class MathMLTableFormatter : public Object
{
protected:
  MathMLTableFormatter(void);
  virtual ~MathMLTableFormatter();

public:
  static SmartPtr<MathMLTableFormatter> create(void)
  { return new MathMLTableFormatter(); }

  void init(const class FormattingContext&,
	    unsigned, unsigned,
	    const std::vector<SmartPtr<MathMLTableCellElement> >&,
	    const std::vector<SmartPtr<MathMLTableCellElement> >&,
	    const SmartPtr<Value>&,
	    const SmartPtr<Value>&,
	    const SmartPtr<Value>&,
	    const SmartPtr<Value>&,
	    const SmartPtr<Value>&,
	    const SmartPtr<Value>&,
	    const SmartPtr<Value>&,
	    const SmartPtr<Value>&,
	    const SmartPtr<Value>&,
	    const SmartPtr<Value>&);
  AreaRef formatLines(const class FormattingContext&,
		      const SmartPtr<Value>&,
		      const SmartPtr<Value>&,
		      const SmartPtr<Value>&) const;
  void formatCells(const class FormattingContext&,
		   const scaled&,
		   const SmartPtr<Value>&) const;
  BoundingBox format(std::vector<BoxedLayoutArea::XYArea>&);

private:
  class Cell
  {
  public:
    Cell(void) { }

    AreaRef getArea(void) const { return content->getArea(); }
    BoundingBox getBoundingBox(void) const { return getArea()->box(); }
    SmartPtr<MathMLTableCellElement> getContent(void) const { return content; }
    bool isNull(void) const { return content == 0; }
    operator bool(void) const { return !isNull(); }
    unsigned getColumnSpan(void) const { return content ? content->getColumnSpan() * 2 - 1 : 0; }
    unsigned getRowSpan(void) const { return content ? content->getRowSpan() * 2 - 1 : 0; }
    TokenId getRowAlign(void) const { return content->getRowAlign(); }
    TokenId getColumnAlign(void) const { return content->getColumnAlign(); }
    void getDisplacement(scaled& x, scaled& y) const { content->getDisplacement(x, y); }
    void setContent(const SmartPtr<MathMLTableCellElement>& c) { content = c; }
    void setDisplacement(const scaled& x, const scaled& y) const { content->setDisplacement(x, y); }

  private:
    SmartPtr<MathMLTableCellElement> content;
  };

  class Row
  {
  public:
    enum RowHeightSpec { AUTO, FIX, SCALE };

    Row(void)
      : contentRow(false), spec(AUTO), fixHeight(), scaleHeight(0.0f),
	tempHeight(), tempDepth(), height(), depth(), displacement()
    { }

    RowHeightSpec getSpec(void) const { return spec; }
    bool isContentRow(void) const { return contentRow; }
    float getScaleHeight(void) const { return scaleHeight; }
    scaled getDepth(void) const { return depth; }
    scaled getFixHeight(void) const { return fixHeight; }
    scaled getHeight(void) const { return height; }
    scaled getTempDepth(void) const { return tempDepth; }
    scaled getTempHeight(void) const { return tempHeight; }
    scaled getVerticalExtent(void) const { return getHeight() + getDepth(); }
    void setContentRow(bool b = true) { contentRow = b; }
    void setDepth(const scaled& d) { depth = d; }
    void setHeight(const scaled& h) { height = h; }
    void setHeightSpec(RowHeightSpec s) { spec = s; }
    void setHeightSpec(const scaled& w) { spec = FIX; fixHeight = w; }
    void setHeightSpec(float s) { spec = SCALE; scaleHeight = s; }
    void setHeightSpec(const class FormattingContext&, const class Length&);
    void setTempDepth(const scaled& d) { tempDepth = d; }
    void setTempHeight(const scaled& h) { tempHeight = h; }
    void setDisplacement(const scaled& d) { displacement = d; }
    scaled getDisplacement(void) const { return displacement; }
    scaled getTopDisplacement(void) const { return getDisplacement() + height; }
    scaled getBottomDisplacement(void) const { return getDisplacement() - depth; }
    scaled getCenterDisplacement(void) const { return getDisplacement() + (height - depth) / 2; }

  private:
    bool contentRow;
    RowHeightSpec spec;
    scaled fixHeight;
    float scaleHeight;
    scaled tempHeight;
    scaled tempDepth;
    scaled height;
    scaled depth;
    scaled displacement;
  };

  class Column
  {
  public:
    enum ColumnWidthSpec { AUTO, FIT, FIX, SCALE };

    Column(void) 
      : contentColumn(false), spec(AUTO), fixWidth(), scaleWidth(0.0f),
	tempWidth(), width(), contentWidth(), displacement()
    { }

    ColumnWidthSpec getSpec(void) const { return spec; }
    bool isContentColumn(void) const { return contentColumn; }
    float getScaleWidth(void) const { return scaleWidth; }
    scaled getContentWidth(void) const { return contentWidth; }
    scaled getFixWidth(void) const { return fixWidth; }
    scaled getTempWidth(void) const { return tempWidth; }
    scaled getWidth(void) const { return width; }
    void setContentColumn(bool b = true) { contentColumn = b; }
    void setContentWidth(const scaled& w) { contentWidth = w; }
    void setTempWidth(const scaled& w) { tempWidth = w; }
    void setWidth(const scaled& w) { width = w; }
    void setWidthSpec(ColumnWidthSpec s) { spec = s; }
    void setWidthSpec(const scaled& w) { spec = FIX; fixWidth = w; }
    void setWidthSpec(float s) { spec = SCALE; scaleWidth = s; }
    void setWidthSpec(const class FormattingContext&, const class Length&);
    void setDisplacement(const scaled& d) { displacement = d; }
    scaled getDisplacement(void) const { return displacement; }
    scaled getLeftDisplacement(void) const { return getDisplacement(); }
    scaled getRightDisplacement(void) const { return getDisplacement() + getWidth(); }
    scaled getCenterDisplacement(void) const { return getDisplacement() + getWidth() / 2; }

  private:
    bool contentColumn;
    ColumnWidthSpec spec;
    scaled fixWidth;
    float scaleWidth;
    scaled tempWidth;
    scaled width;
    scaled contentWidth;
    scaled displacement;
  };

protected:
  const Cell& getCell(unsigned, unsigned) const;
  BoundingBox getBoundingBox(void) const { return BoundingBox(getWidth(), getHeight(), getDepth()); }
  BoundingBox getCellBoundingBox(unsigned, unsigned, unsigned, unsigned) const;
  scaled computeTableHeightDepthF(void);
  scaled computeTableHeightDepthT(void);
  scaled computeMinimumTableWidthF(void);
  scaled computeMinimumTableWidthT(void);
  scaled computeMinimumTableWidth(void);
  scaled computeTableWidth(const scaled&);
  BoundingBox assignTableWidth(const scaled&);
  void assignTableWidthT(const scaled&);
  void assignTableWidthF(const scaled&);
  scaled getColumnContentWidth(unsigned) const;
  scaled getWidth(void) const { return width; }
  scaled getHeight(void) const { return height; }
  scaled getDepth(void) const { return depth; }
  void alignTable(const scaled&, const scaled&, TokenId);
  void alignTable(const scaled&, const scaled&, TokenId, unsigned);
  void initTempHeightDepth();
  void initTempWidths(void);
  void setDisplacements(void);
  void setCellPosition(void);
  void setWidth(const scaled& w) { width = w; }
  void setHeight(const scaled& h) { height = h; }
  void setDepth(const scaled& d) { depth = d; }

  scaled axis;

  unsigned nRows;
  unsigned nColumns;
  int numCol; // nContentColumns
  scaled sumFix;
  scaled sumCont;
  float sumScale;
  bool equalRows;
  bool equalColumns;
  TokenId tableAlign;
  int tableAlignRow;

  scaled width;
  scaled height;
  scaled depth;
  std::vector<Row> rows;
  std::vector<Column> columns;
  std::vector<Cell> cells;
};

#endif // __MathMLTableFormatter_hh__
