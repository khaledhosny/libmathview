// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef MathMLTableElement_hh
#define MathMLTableElement_hh

#include "MathMLTableCellElement.hh"
#include "MathMLTableRowElement.hh"
#include "MathMLLinearContainerElement.hh"
#include "MathMLAlignGroupElement.hh"

#define MIN_COLUMN_SPACING (px2sp(3))

class MathMLTableCellElement;     // forward declaration to avoid circular dependencies

enum WidthId {
  WIDTH_AUTO,
  WIDTH_FIXED,
  WIDTH_PERCENTAGE
};

enum SpacingId {
  SPACING_FIXED,
  SPACING_PERCENTAGE
};

enum ColumnWidthId {
  COLUMN_WIDTH_AUTO,
  COLUMN_WIDTH_FIXED,
  COLUMN_WIDTH_PERCENTAGE,
  COLUMN_WIDTH_FIT
};

struct GroupExtent {
  scaled left;  // left width (from group start to alignment point)
  scaled right; // right width (from alignment point to group end)
};

struct AlignmentGroup {
  AlignmentGroup(void) { Reset(); }

  void Reset(void)
  {
    group = 0;
    alignment = T__NOTVALID;
    leftEdge = rightEdge = extent.left = extent.right = 0;
  }

  MathMLAlignGroupElement* group; // ptr to the element
  TokenId alignment;         // alignment for this group
  scaled leftEdge;                // left edge (different from left width)
  scaled rightEdge;               // right edge (different from right width)
  GroupExtent extent;
};

struct TableCell {
  TableCell(void) { Reset(); }

  void Reset(void)
  {
    spanned = false;
    rowSpan = colSpan = 0;
    nAlignGroup = 0;
    aGroup = 0;
    rowAlign = T__NOTVALID;
    columnAlign = T__NOTVALID;
    iGroup = 0;
  }

  // phase 1: MathMLTableElement::SetupTable
  SmartPtr<MathMLTableCellElement> mtd;  // ptr to <mtd> element
  bool     spanned;             // true if mtd is a pointer to the spanning cell
  unsigned rowSpan;             // spanning rows
  unsigned colSpan;             // spanning columns
  
  // phase 2: MathMLTableCellElement::SetupGroups
  unsigned nAlignGroup;         // number of alignment groups
  AlignmentGroup* aGroup;

  TokenId rowAlign;
  TokenId columnAlign;

  // following fields are only temporarily used while arranging groups
  unsigned iGroup;                // group index
  SmartPtr<MathMLAlignGroupElement> group; // last group found

  // some facilities
  bool ColumnSpanning(void) { return mtd && !spanned && colSpan > 1; }
  bool RowSpanning(void) { return mtd && !spanned && rowSpan > 1; }
};

typedef TableCell* TableCellPtr;
typedef TableCellPtr* TableCellMatrix;

struct TableColumn {
  TableColumn(void) { Reset(); }

  void Reset(void)
  {
    nAlignGroup = 0;
    widthType = COLUMN_WIDTH_AUTO;
    fixedWidth = 0;
    spacingType = SPACING_FIXED;
    fixedSpacing = 0;
    lineType = T__NOTVALID;
    contentWidth = minimumWidth = width = spacing = 0;
  }

  // phase 3: MathMLTableElement::arrangeGroupsAlignment
  unsigned nAlignGroup;         // number of alignment groups within this column

  ColumnWidthId widthType;      // type of width computation for this column
  // NOTE: had to remove the union since the new scaled data type
  // has a constructor
  scaled      fixedWidth;     // when widthType = COLUMN_WIDTH_FIXED
  float       scaleWidth;     // width (when widthType == % )

  SpacingId   spacingType;    // type of spacing
  // NOTE: same as above
  scaled      fixedSpacing;   // spacing between columns (spacingType == FIXED)
  float       scaleSpacing;   // spacing (spacingType == %)
  
  TokenId     lineType;       // line type between columns

  scaled      contentWidth;   // minimum width for actual content (set by RenderColumn)
  scaled      minimumWidth;   // minimum width for this column
  scaled      width;          // width of last rendering (>= contentWidth)
  scaled      spacing;
};

struct TableRow {
  TableRow(void) { Reset(); }

  void Reset(void)
  {
    spacingType = SPACING_FIXED;
    fixedSpacing = 0;
    lineType = T__NOTVALID;
    height = depth = spacing = 0;
  }

  SmartPtr<MathMLTableRowElement> mtr;   // Table Row element

  SpacingId spacingType;      // type of spacing (absolute or %)
  scaled    fixedSpacing;     // spacing between rows (spacingType == FIXED)
  float     scaleSpacing;     // spacing between rows (spacingType == %)

  TokenId   lineType;         // line type between rows

  scaled    height;
  scaled    depth;
  scaled    verticalExtent(void) const { return height + depth; }
  scaled    spacing;
};

struct RowLabel {
  RowLabel(void) { Reset(); }

  void Reset(void)
  {
    rowAlign = T__NOTVALID;
    columnAlign = T__NOTVALID;
  }

  SmartPtr<MathMLElement> labelElement;
  TokenId rowAlign;
  TokenId columnAlign;
};

class MathMLTableElement
  : public MathMLLinearContainerElement
{
protected:
  MathMLTableElement(const SmartPtr<class MathMLView>&);
  virtual ~MathMLTableElement();

public:
  static SmartPtr<MathMLTableElement> create(const SmartPtr<class MathMLView>& view)
  { return new MathMLTableElement(view); }

  virtual void construct(void);
  virtual void refine(class AbstractRefinementContext&);
#if 0
  virtual void Setup(class RenderingEnvironment&);
  virtual void DoLayout(const class FormattingContext&);
  virtual void SetPosition(const scaled&, const scaled&);
  virtual void Render(const class DrawingArea&);
  virtual void ReleaseGCs(void);

  virtual SmartPtr<MathMLElement> Inside(const scaled& x, const scaled& y);
#endif

  void 	       SetupColumnAlignAux(const SmartPtr<Value>&, unsigned, unsigned, bool = false);
  void 	       SetupRowAlignAux(const SmartPtr<Value>&, unsigned, bool = false);
  void 	       SetupGroupAlignAux(const SmartPtr<Value>&, unsigned, unsigned);

  bool         HasLabels(void) const { return rowLabel != NULL; }

  virtual void setDirtyAttribute(void)
  {
    MathMLLinearContainerElement::setDirtyAttribute();
    setDirtyStructure();
  };

protected:
  TableCell*   GetCell(unsigned r, unsigned c) { return &cell[r][c]; }
  void         RenderTableBackground(const class DrawingArea&);

  // table setup
  void 	       CalcTableSize(void);
  void 	       SetupCellSpanning(class RenderingEnvironment&);
  void 	       SetupCells(void);
  void 	       SetupColumns(class RenderingEnvironment&);
  void 	       SetupAlignmentScopes(class RenderingEnvironment&);
  void 	       SetupColumnAlign(class RenderingEnvironment&);
  void 	       SetupRows(class RenderingEnvironment&);
  void 	       SetupRowAlign(class RenderingEnvironment&);
  void         SetupLabels(void);
  void 	       SetupGroups(void);
  void 	       SetupGroupAlign(class RenderingEnvironment&);
  void 	       SetupAlignMarks(void);
  void 	       SetupTableAttributes(class RenderingEnvironment&);
  void 	       AlignTable(const scaled&, BoundingBox&);
#if 0
  void         ReleaseAuxStructures(void);
#endif

  // table layout
  void         DoHorizontalLayout(const class FormattingContext&);
  void         DoHorizontalMinimumLayout(void);
  // void         DoVerticalLayout(FormattingContext::LayoutId);
  void         ConfirmHorizontalFixedSpacing(void);
  void         ConfirmHorizontalScaleSpacing(const scaled&);
  void         ConfirmVerticalFixedSpacing(void);
  void         ConfirmVerticalScaleSpacing(const scaled&);
  void         AdjustTableWidth(const scaled&);
  // void         SpanRowHeight(FormattingContext::LayoutId);
  void         ColumnLayout(unsigned, const class FormattingContext&);
  void         ScaleColumnsLayout(const class FormattingContext&);
  void         SpannedCellsLayout(const class FormattingContext&);
  void         StretchyCellsLayout(void);
  scaled       ColumnGroupsLayout(unsigned, const class FormattingContext&);
  scaled       PrepareLabelsLayout(const class FormattingContext&);
  void         DoLabelsLayout(const class FormattingContext&);
  void         AdjustTableLayoutWithLabels(const class FormattingContext&);
  void         SetLabelPosition(unsigned, const scaled&, const scaled&);

  // table invariants
  void         EnforceHorizontalInvariants(void);
  void         EnforceVerticalInvariants(void);
  void         NormalizeHorizontalScale(float);
  void         NormalizeVerticalScale(float);

  // measures
  scaled       GetSpacingWidth(void) const;
  scaled       GetSpacingWidth(SpacingId) const;
  scaled       GetMinimumWidth(unsigned, unsigned) const;
  scaled       GetColumnWidth(void) const;
  scaled       GetColumnWidth(ColumnWidthId) const;
  scaled       GetColumnWidth(unsigned, unsigned) const;
  scaled       GetContentWidth(void) const;
  scaled       GetContentWidth(ColumnWidthId) const;
  scaled       GetRowHeight(void) const;
  scaled       GetRowHeight(unsigned, unsigned) const;
  scaled       GetSpacingHeight(void) const;
  scaled       GetSpacingHeight(SpacingId) const;
  scaled       GetTableWidth(void) const;
  scaled       GetTableHeight(void) const;
  scaled       GetMaxLabelWidth(void) const;
  unsigned     CountColumnTypes(ColumnWidthId) const;
  unsigned     CountHorizontalSpacingTypes(SpacingId) const;
  float        GetHorizontalScale(void) const;
  float        GetVerticalScale(void) const;

private:
  unsigned      nRows;     // total # of rows in the table (tipically, content->getSize())
  unsigned      nColumns;  // total # of columns in the table
  TableCellMatrix cell;    // table data

  TableColumn* column;
  TableRow*    row;

  TokenId      frame;

  TokenId      align;
  int          rowNumber;

  WidthId      widthType;
  scaled       fixedWidth;
  float        scaleWidth;
  scaled       width;

  SpacingId    frameHorizontalSpacingType;
  scaled       frameHorizontalFixedSpacing;
  float        frameHorizontalScaleSpacing;
  scaled       frameHorizontalSpacing; // actual orizontal spacing

  SpacingId    frameVerticalSpacingType;
  scaled       frameVerticalFixedSpacing;
  float        frameVerticalScaleSpacing;
  scaled       frameVerticalSpacing; // actual vertical spacing

  bool         equalRows;
  bool         equalColumns;
  bool         displayStyle;

  TokenId      side;

  SpacingId    minLabelSpacingType;
  scaled       minLabelFixedSpacing;
  float        minLabelScaleSpacing;
  scaled       minLabelSpacing;

  // bool         overlappingLabels;
  RowLabel*    rowLabel;
  scaled       labelsWidth;
  scaled       tableWidth;
  scaled       leftPadding;

  RGBColor     color; // color for frame and lines (if present)
  scaled       lineThickness;
#if 0
  const GraphicsContext* dGC[2]; // GC for dashed lines
#endif

  scaled        environmentAxis; // for axis alignment

  // the following fields are computed during minimumLayout

  unsigned      nAuto;
  unsigned      nFit;

  float         wScale;
  float         hScale;
};

#endif // MathMLTableElement_hh
