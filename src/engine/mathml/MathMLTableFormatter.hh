// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#ifndef __MathMLTableFormatter_hh__
#define __MathMLTableFormatter_hh__

#include <vector>

#include "token.hh"
#include "Object.hh"
#include "SmartPtr.hh"
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
	    const SmartPtr<Value>&);
  AreaRef formatLines(const class FormattingContext&,
		      const SmartPtr<Value>&,
		      const SmartPtr<Value>&) const;
  void formatCells(const class FormattingContext&,
		   const scaled&,
		   const SmartPtr<Value>&) const;
  AreaRef format(const class FormattingContext&,
		 unsigned,
		 const SmartPtr<Value>&,
		 const SmartPtr<Value>&,
		 const SmartPtr<Value>&);

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
    void setTempDepth(const scaled& d) { tempDepth = d; }
    void setTempHeight(const scaled& h) { tempHeight = h; }
    void setDisplacement(const scaled& d) { displacement = d; }
    scaled getDisplacement(void) const { return displacement; }

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
    void setDisplacement(const scaled& d) { displacement = d; }
    scaled getDisplacement(void) const { return displacement; }

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
  BoundingBox getCellBoundingBox(unsigned, unsigned, unsigned, unsigned) const;
  scaled calcTableHeightDepthF(void);
  scaled calcTableHeightDepthT(int&, float&, scaled&, scaled&);
  scaled calcTableWidthF(int&, scaled&, scaled&, float&);
  scaled calcTableWidthT(int&, scaled&, float&);
  scaled getColumnContentWidth(unsigned) const;
  scaled getWidth(void) const { return width; }
  scaled getHeight(void) const { return height; }
  scaled getDepth(void) const { return depth; }
  scaled initHeightsF(void);
  scaled initHeightsT(void);
  void alignTable(const scaled&, const scaled&, TokenId);
  void alignTable(const scaled&, const scaled&, TokenId, unsigned);
  void initTempHeights(void);
  void initTempWidths(void);
  void initWidthsF(void);
  void initWidthsT(void);
  void setCellPositions(const scaled&);
  void setWidth(const scaled& w) { width = w; }
  void setHeight(const scaled& h) { height = h; }
  void setDepth(const scaled& d) { depth = d; }

  scaled width;
  scaled height;
  scaled depth;
  std::vector<Row> rows;
  std::vector<Column> columns;
  std::vector<Cell> cells;
};

#endif // __MathMLTableFormatter_hh__
