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

#ifndef MathMLTableElement_hh
#define MathMLTableElement_hh

#include "MathMLTableRowElement.hh"
#include "MathMLContainerElement.hh"
#include "MathMLAlignGroupElement.hh"

#define MIN_COLUMN_SPACING (float2sp(3 * SCALED_POINTS_PER_PX))

class MathMLTableCellElement;     // forward declaration to avoid circular dependencies

enum WidthId {
  WIDTH_AUTO,
  WIDTH_FIXED,
  WIDTH_PERCENTAGE
};

struct GroupExtent {
  scaled left;  // left width (from group start to alignment point)
  scaled right; // right width (from alignment point to group end)
};

struct AlignmentGroup {
  MathMLAlignGroupElement* group; // ptr to the element
  GroupAlignId alignment;         // alignment for this group
  scaled leftEdge;                // left edge (different from left width)
  scaled rightEdge;               // right edge (different from right width)
  GroupExtent extent;
};

struct TableCell {
  // phase 1: MathMLTableElement::SetupTable
  MathMLTableCellElement* mtd;  // ptr to <mtd> element
  bool     spanned;             // true if mtd is a pointer to the spanning cell
  unsigned rowSpan;             // spanning rows
  unsigned colSpan;             // spanning columns
  
  // phase 2: MathMLTableCellElement::SetupGroups
  unsigned nAlignGroup;         // number of alignment groups
  AlignmentGroup* aGroup;

  RowAlignId rowAlign;
  ColumnAlignId columnAlign;

  // following fields are only temporarily used while arranging groups
  unsigned iGroup;                // group index
  MathMLAlignGroupElement* group; // last group found

  // some facilities
  bool ColumnSpanning(void) { return mtd != NULL && !spanned && colSpan > 1; }
  bool RowSpanning(void) { return mtd != NULL && !spanned && rowSpan > 1; }
};

typedef TableCell* TableCellPtr;
typedef TableCellPtr* TableCellMatrix;

struct TableColumn {
  // phase 3: MathMLTableElement::arrangeGroupsAlignment
  unsigned nAlignGroup;         // number of alignment groups within this column

  ColumnWidthId widthType;      // type of width computation for this column
  union {
    scaled      fixedWidth;     // when widthType = COLUMN_WIDTH_FIXED
    float       scaleWidth;     // width (when widthType == % )
  };

  SpacingId     spacingType;    // type of spacing
  union {
    scaled      fixedSpacing;   // spacing between columns (spacingType == FIXED)
    float       scaleSpacing;   // spacing (spacingType == %)
  };
  
  TableLineId   lineType;       // line type between columns

  scaled        contentWidth;   // minimum width for actual content (set by RenderColumn)
  scaled        minimumWidth;   // minimum width for this column
  scaled        width;          // width of last rendering (>= contentWidth)
  scaled        spacing;
};

struct TableRow {
  MathMLTableRowElement* mtr;   // Table Row element

  SpacingId   spacingType;      // type of spacing (absolute or %)
  union {
    scaled    fixedSpacing;     // spacing between rows (spacingType == FIXED)
    float     scaleSpacing;     // spacing between rows (spacingType == %)
  };

  TableLineId lineType;         // line type between rows

  scaled      ascent;
  scaled      descent;
  scaled      GetHeight(void) const { return ascent + descent; }
  scaled      spacing;
};

struct RowLabel {
  MathMLElement* labelElement;
  RowAlignId     rowAlign;
  ColumnAlignId  columnAlign;
};

class MathMLTableElement: public MathMLContainerElement {
public:
  MathMLTableElement(mDOMNodeRef);
  virtual const AttributeSignature* GetAttributeSignature(AttributeId) const;
  virtual void Normalize(void);
  virtual void Setup(class RenderingEnvironment*);
  virtual void DoBoxedLayout(LayoutId, BreakId, scaled);
  virtual void SetPosition(scaled, scaled);
  virtual void Render(const class DrawingArea&);
  virtual void ReleaseGCs(void);
  virtual ~MathMLTableElement();

  virtual MathMLElement* Inside(scaled x, scaled y);

  void 	       SetupColumnAlignAux(const Value*, unsigned, unsigned, bool = false);
  void 	       SetupRowAlignAux(const Value*, unsigned, bool = false);
  void 	       SetupGroupAlignAux(const Value*, unsigned, unsigned);

  virtual void SetDirty(const Rectangle* rect);
  virtual bool IsExpanding(void) const;
  bool         HasLabels(void) const { return rowLabel != NULL; }

protected:
  TableCell*   GetCell(unsigned r, unsigned c) { return &cell[r][c]; }
  void         RenderTableBackground(const class DrawingArea&);

  // table setup
  void 	       CalcTableSize(void);
  void 	       SetupCellSpanning(class RenderingEnvironment*);
  void 	       SetupCells(void);
  void 	       SetupColumns(class RenderingEnvironment*);
  void 	       SetupAlignmentScopes(class RenderingEnvironment*);
  void 	       SetupColumnAlign(class RenderingEnvironment*);
  void 	       SetupRows(class RenderingEnvironment*);
  void 	       SetupRowAlign(class RenderingEnvironment*);
  void         SetupLabels(void);
  void 	       SetupGroups(void);
  void 	       SetupGroupAlign(class RenderingEnvironment*);
  void 	       SetupAlignMarks(void);
  void 	       SetupTableAttributes(class RenderingEnvironment*);
  void 	       AlignTable(scaled, BoundingBox&);
  void         ReleaseAuxStructures(void);

  // table layout
  void         DoHorizontalLayout(LayoutId, BreakId, scaled);
  void         DoHorizontalMinimumLayout(void);
  void         DoVerticalLayout(LayoutId);
  void         ConfirmHorizontalFixedSpacing(void);
  void         ConfirmHorizontalScaleSpacing(scaled);
  void         ConfirmVerticalFixedSpacing(void);
  void         ConfirmVerticalScaleSpacing(scaled);
  void         AdjustTableWidth(scaled);
  void         SpanRowHeight(LayoutId);
  void         ColumnLayout(unsigned, LayoutId, BreakId, scaled);
  void         ScaleColumnsLayout(LayoutId, BreakId, scaled);
  void         SpannedCellsLayout(LayoutId);
  void         StretchyCellsLayout(void);
  scaled       ColumnGroupsLayout(unsigned, LayoutId);
  scaled       PrepareLabelsLayout(LayoutId, scaled);
  void         DoLabelsLayout(LayoutId, scaled);
  void         AdjustTableLayoutWithLabels(LayoutId, scaled);
  void         SetLabelPosition(unsigned, scaled, scaled);

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

  TableColumn*  column;
  TableRow*     row;

  TableLineId   frame;

  TableAlignId  align;
  int           rowNumber;

  WidthId       widthType;
  union {
    scaled      fixedWidth;
    float       scaleWidth;
  };
  scaled        width;

  SpacingId     frameHorizontalSpacingType;
  union {
    scaled      frameHorizontalFixedSpacing;
    float       frameHorizontalScaleSpacing;
  };
  scaled        frameHorizontalSpacing; // actual orizontal spacing

  SpacingId     frameVerticalSpacingType;
  union {
    scaled      frameVerticalFixedSpacing;
    float       frameVerticalScaleSpacing;
  };
  scaled        frameVerticalSpacing; // actual vertical spacing

  bool          equalRows;
  bool          equalColumns;
  bool          displayStyle;

  TableSideId   side;

  SpacingId     minLabelSpacingType;
  union {
    scaled      minLabelFixedSpacing;
    float       minLabelScaleSpacing;
  };
  scaled        minLabelSpacing;

  // bool          overlappingLabels;
  RowLabel*     rowLabel;
  scaled        labelsWidth;
  scaled        tableWidth;
  scaled        leftPadding;

  RGBValue      color; // color for frame and lines (if present)
  scaled        lineThickness;
  const GraphicsContext* dGC[2]; // GC for dashed lines

  scaled        environmentAxis; // for axis alignment

  // the following fields are computed during minimumLayout

  unsigned      nAuto;
  unsigned      nFit;

  float         wScale;
  float         hScale;
};

#define TO_TABLE(object) (dynamic_cast<MathMLTableElement*>(object))

#endif // MathMLTableElement_hh
