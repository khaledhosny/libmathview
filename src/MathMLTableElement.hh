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

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLTableCellElement.hh"
#include "MathMLTableRowElement.hh"
#include "MathMLLinearContainerElement.hh"
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
  AlignmentGroup(void) { Reset(); }

  void Reset(void)
  {
    group = 0;
    alignment = GROUP_ALIGN_NOTVALID;
    leftEdge = rightEdge = extent.left = extent.right = 0;
  }

  MathMLAlignGroupElement* group; // ptr to the element
  GroupAlignId alignment;         // alignment for this group
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
    rowAlign = ROW_ALIGN_NOTVALID;
    columnAlign = COLUMN_ALIGN_NOTVALID;
    iGroup = 0;
  }

  // phase 1: MathMLTableElement::SetupTable
  Ptr<MathMLTableCellElement> mtd;  // ptr to <mtd> element
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
  Ptr<MathMLAlignGroupElement> group; // last group found

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
    widthType = COLUMN_WIDTH_NOTVALID;
    fixedWidth = 0;
    spacingType = SPACING_NOTVALID;
    fixedSpacing = 0;
    lineType = TABLE_LINE_NOTVALID;
    contentWidth = minimumWidth = width = spacing = 0;
  }

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
  TableRow(void) { Reset(); }

  void Reset(void)
  {
    spacingType = SPACING_NOTVALID;
    fixedSpacing = 0;
    lineType = TABLE_LINE_NOTVALID;
    ascent = descent = spacing = 0;
  }

  Ptr<MathMLTableRowElement> mtr;   // Table Row element

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
  RowLabel(void) { Reset(); }

  void Reset(void)
  {
    rowAlign = ROW_ALIGN_NOTVALID;
    columnAlign = COLUMN_ALIGN_NOTVALID;
  }

  Ptr<MathMLElement> labelElement;
  RowAlignId     rowAlign;
  ColumnAlignId  columnAlign;
};

class MathMLTableElement
  : public MathMLLinearContainerElement
{
protected:
  MathMLTableElement(void);
#if defined(HAVE_GMETADOM)
  MathMLTableElement(const DOM::Element&);
#endif
  virtual ~MathMLTableElement();

private:
  void Init(void);

public:
  static Ptr<MathMLElement> create(void)
  { return Ptr<MathMLElement>(new MathMLTableElement()); }
#if defined(HAVE_GMETADOM)
  static Ptr<MathMLElement> create(const DOM::Element& el)
  { return Ptr<MathMLElement>(new MathMLTableElement(el)); }
#endif

  virtual const AttributeSignature* GetAttributeSignature(AttributeId) const;
  virtual void Normalize(const Ptr<class MathMLDocument>&);
  virtual void Setup(class RenderingEnvironment&);
  virtual void DoLayout(const class FormattingContext&);
  virtual void SetPosition(scaled, scaled);
  virtual void Render(const class DrawingArea&);
  virtual void ReleaseGCs(void);

  virtual Ptr<MathMLElement> Inside(scaled x, scaled y);

  void 	       SetupColumnAlignAux(const Value*, unsigned, unsigned, bool = false);
  void 	       SetupRowAlignAux(const Value*, unsigned, bool = false);
  void 	       SetupGroupAlignAux(const Value*, unsigned, unsigned);

  //virtual void SetDirty(const Rectangle* rect);
  bool         HasLabels(void) const { return rowLabel != NULL; }

  virtual void SetDirtyAttribute(void)
  {
    MathMLLinearContainerElement::SetDirtyAttribute();
    SetDirtyStructure();
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
  void 	       AlignTable(scaled, BoundingBox&);
  void         ReleaseAuxStructures(void);

  // table layout
  void         DoHorizontalLayout(const class FormattingContext&);
  void         DoHorizontalMinimumLayout(void);
  void         DoVerticalLayout(LayoutId);
  void         ConfirmHorizontalFixedSpacing(void);
  void         ConfirmHorizontalScaleSpacing(scaled);
  void         ConfirmVerticalFixedSpacing(void);
  void         ConfirmVerticalScaleSpacing(scaled);
  void         AdjustTableWidth(scaled);
  void         SpanRowHeight(LayoutId);
  void         ColumnLayout(unsigned, const class FormattingContext&);
  void         ScaleColumnsLayout(const class FormattingContext&);
  void         SpannedCellsLayout(const class FormattingContext&);
  void         StretchyCellsLayout(void);
  scaled       ColumnGroupsLayout(unsigned, const class FormattingContext&);
  scaled       PrepareLabelsLayout(const class FormattingContext&);
  void         DoLabelsLayout(const class FormattingContext&);
  void         AdjustTableLayoutWithLabels(const class FormattingContext&);
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

#endif // MathMLTableElement_hh
