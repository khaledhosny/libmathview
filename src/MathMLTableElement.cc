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

#include <algorithm>
#include <functional>

#include <assert.h>
#include <stddef.h>

#include "Adaptors.hh"
#include "ChildList.hh"
#include "StringUnicode.hh"
#include "MathMLDocument.hh"
#include "MathMLTableElement.hh"
#include "MathMLTableCellElement.hh"

MathMLTableElement::MathMLTableElement()
{
  Init();
}

#if defined(HAVE_GMETADOM)
MathMLTableElement::MathMLTableElement(const DOM::Element& node)
  : MathMLLinearContainerElement(node)
{
  Init();
}
#endif

void
MathMLTableElement::Init()
{
  nRows    = 0;
  nColumns = 0;
  cell     = NULL;
  column   = NULL;
  row      = NULL;
  rowLabel = NULL;
  width    = 0;

  dGC[0] = dGC[1] = NULL;
}

const AttributeSignature*
MathMLTableElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_ALIGN,           tableAlignParser,         new StringC("axis"),        NULL },
    { ATTR_ROWALIGN,        rowAlignListParser,       new StringC("baseline"),    NULL },
    { ATTR_COLUMNALIGN,     columnAlignListParser,    new StringC("center"),      NULL },
    { ATTR_GROUPALIGN,      groupAlignListListParser, new StringC("{ left }"),    NULL },
    { ATTR_ALIGNMENTSCOPE,  booleanListParser,        new StringC("true"),        NULL },
    { ATTR_COLUMNWIDTH,     columnWidthListParser,    new StringC("auto"),        NULL },
    { ATTR_ROWSPACING,      numberUnitListParser,     new StringC("1.0ex"),       NULL },
    { ATTR_COLUMNSPACING,   spaceListParser,          new StringC("0.8em"),       NULL },
    { ATTR_ROWLINES,        lineTypeListParser,       new StringC("none"),        NULL },
    { ATTR_COLUMNLINES,     lineTypeListParser,       new StringC("none"),        NULL },
    { ATTR_FRAME,           lineTypeParser,           new StringC("none"),        NULL },
    { ATTR_FRAMESPACING,    tableFrameSpacingParser,  new StringC("0.4em 0.5ex"), NULL },
    { ATTR_EQUALROWS,       booleanParser,            new StringC("false"),       NULL },
    { ATTR_EQUALCOLUMNS,    booleanParser,            new StringC("false"),       NULL },
    { ATTR_DISPLAYSTYLE,    booleanParser,            new StringC("false"),       NULL },
    { ATTR_SIDE,            tableSideParser,          new StringC("right"),       NULL },
    { ATTR_MINLABELSPACING, numberUnitParser,         new StringC("0.8em"),       NULL },
    { ATTR_WIDTH,           tableWidthParser,         new StringC("auto"),        NULL },
    { ATTR_NOTVALID,        NULL,                     NULL,                       NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLLinearContainerElement::GetAttributeSignature(id);

  return signature;
}

MathMLTableElement::~MathMLTableElement()
{
  ReleaseAuxStructures();
}

void
MathMLTableElement::Normalize(const Ptr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (GetDOMElement())
	{
	  ChildList children(GetDOMElement(), MATHML_NS_URI, "*");
	  unsigned n = children.get_length();

	  std::vector< Ptr<MathMLElement> > newContent;
	  newContent.reserve(n);
	  for (unsigned i = 0; i < n; i++)
	    {
	      DOM::Element node = children.item(i);
	      assert(node);
	      if (nodeLocalName(node) == "mtr" || nodeLocalName(node) == "mlabeledtr")
		{
		  Ptr<MathMLElement> elem = doc->getFormattingNode(node);
		  assert(elem);
		  newContent.push_back(elem);
		}
	      else
		{
		  // ISSUE WARNING
		}
	    }
	  SwapChildren(newContent);
	}
#endif // HAVE_GMETADOM

      if (content.size() == 0)
	Append(smart_cast<MathMLTableRowElement>(MathMLTableRowElement::create()));

      std::for_each(content.begin(), content.end(), std::bind2nd(NormalizeAdaptor(), doc));

      ResetDirtyStructure();
    }
}

void
MathMLTableElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;

  //printf("setposition for table (%d,%d) %d rows\n", sp2ipx(x), sp2ipx(y), nRows);

  scaled yOffset = frameVerticalSpacing - box.ascent;

  for (unsigned i = 0; i < nRows; i++) {
    scaled xOffset = frameHorizontalSpacing;

    if (HasLabels()) {
      if (rowLabel[i].labelElement &&
	  (side == TABLE_SIDE_LEFT || side == TABLE_SIDE_LEFTOVERLAP))
	  SetLabelPosition(i, x, y + yOffset + row[i].ascent);
      
      xOffset += leftPadding;
    }

    //printf("row set position %d %d height = %d\n", sp2ipx(x + xOffset), sp2ipx(y + yOffset + row[i].ascent), sp2ipx(row[i].GetHeight() + row[i].spacing));

    if (row[i].mtr)
      row[i].mtr->SetPosition(x + xOffset, y + yOffset + row[i].ascent);

    for (unsigned j = 0; j < nColumns; j++) {
      TableCell* cell = GetCell(i, j);

      if (cell->mtd && !cell->spanned) {
	const BoundingBox& cellBox = cell->mtd->GetBoundingBox();
	//printf("cell set position %d %d\n", sp2ipx(x + xOffset), sp2ipx(y + yOffset + cellBox.ascent));
	cell->mtd->SetPosition(x + xOffset, y + yOffset + cellBox.ascent);
      }
      
      xOffset += column[j].width;
      if (j < nColumns - 1) xOffset += column[j].spacing;
    }

    if (HasLabels()) {
      xOffset += frameHorizontalSpacing;

      if (rowLabel[i].labelElement &&
	  (side == TABLE_SIDE_RIGHT || side == TABLE_SIDE_RIGHTOVERLAP))
	SetLabelPosition(i, x + xOffset, y + yOffset + row[i].ascent);
    }

    yOffset += row[i].GetHeight() + row[i].spacing;
  }
}

// SetLabelPosition:
// if side == LEFT? then x is the left edge of the whole table
// if side == RIGHT? then x is the right edge of the whole table (and thus
// must be adjust to accomodate the label)
void
MathMLTableElement::SetLabelPosition(unsigned i, scaled x, scaled y)
{
  assert(i < nRows);
  assert(rowLabel);
  assert(rowLabel[i].labelElement);

  const BoundingBox& labelBox = rowLabel[i].labelElement->GetBoundingBox();

  switch (rowLabel[i].rowAlign) {
  case ROW_ALIGN_BOTTOM:
    y += row[i].descent - labelBox.descent;
    break;
  case ROW_ALIGN_CENTER:
    y += (row[i].GetHeight() - labelBox.GetHeight()) / 2 +
      labelBox.ascent - row[i].ascent;
    break;
  case ROW_ALIGN_BASELINE:
    break;
  case ROW_ALIGN_AXIS:
    assert(IMPOSSIBLE);
    break;
  case ROW_ALIGN_TOP:
    y += labelBox.ascent - row[i].ascent;
    break;
  default:
    break;
  }

  scaled columnWidth;
  if (side == TABLE_SIDE_LEFT || side == TABLE_SIDE_LEFTOVERLAP) columnWidth = leftPadding;
  else columnWidth = box.width - leftPadding - tableWidth;

  switch (rowLabel[i].columnAlign) {
  case COLUMN_ALIGN_RIGHT:
    x += columnWidth - labelBox.width;
    break;
  case COLUMN_ALIGN_CENTER:
    x += (columnWidth - labelBox.width) / 2;
    break;
  case COLUMN_ALIGN_LEFT:
  default:
    break;
  }

  rowLabel[i].labelElement->SetPosition(x, y);
}

void
MathMLTableElement::RenderTableBackground(const DrawingArea& area)
{
  if (bGC[Selected()] == NULL) {
    GraphicsContextValues values;
    values.foreground = values.background = Selected() ? area.GetSelectionBackground() : background;
    bGC[Selected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_BACKGROUND);
  }

  if (DirtyBackground())
    area.Clear(bGC[Selected()], GetX(), GetY(), box);

#if 0   
  for (unsigned i = 0; i < nRows; i++) {
    assert(row[i].mtr);
    if (row[i].mtr->Selected()) {
      row[i].mtr->RenderBackground(area);
      return;
    }
  }
#endif
}

void
MathMLTableElement::Render(const DrawingArea& area)
{
  if (Dirty())
    {
      MathMLLinearContainerElement::Render(area);

      if (fGC[Selected()] == NULL) {
	GraphicsContextValues values;
	values.foreground = Selected() ? area.GetSelectionForeground() : color;
	values.lineStyle = LINE_STYLE_SOLID;
	values.lineWidth = lineThickness;
	fGC[Selected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_LINE_STYLE);

	values.lineStyle = LINE_STYLE_DASHED;
	dGC[Selected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_LINE_STYLE);
      }

      //area.DrawRectangle(gc, GetX(), GetY(), box);

      if (frame != TABLE_LINE_NONE) {
	Rectangle rect;

	if (HasLabels()) {
	  rect.x = GetX() + leftPadding;
	  rect.y = GetY() - box.ascent;    
	  rect.width = tableWidth;
	  rect.height = box.GetHeight();
	} else
	  rect = box.GetRectangle(GetX(), GetY());

	area.DrawRectangle((frame == TABLE_LINE_DASHED) ? dGC[Selected()] : fGC[Selected()], rect);
      }

      scaled yOffset = frameVerticalSpacing - box.ascent;
      for (unsigned i = 0; i < nRows; i++) {
	scaled xOffset = frameHorizontalSpacing;
	if (HasLabels()) xOffset += leftPadding;

	for (unsigned j = 0; j < nColumns; j++) {
	  TableCell* cell = GetCell(i, j);

	  if (i != nRows - 1 && row[i].lineType != TABLE_LINE_NONE) {
	    // horizontal lines
	    if (cell->rowSpan <= 1) {
	      scaled lineX = position.x + xOffset;
	      scaled lineY = position.y + yOffset + row[i].GetHeight() + row[i].spacing / 2;
	      scaled len = column[j].width;

	      if (j == 0) {
		lineX -= frameHorizontalSpacing;
		len   += frameHorizontalSpacing;
	      } else {
		lineX -= column[j - 1].spacing / 2;
		len   += column[j - 1].spacing / 2;
	      }

	      if (j == nColumns - 1) {
		len   += frameHorizontalSpacing;
	      } else {
		len   += column[j].spacing / 2;
	      }

	      area.DrawLine((row[i].lineType == TABLE_LINE_DASHED) ? dGC[Selected()] : fGC[Selected()],
			    lineX, lineY, lineX + len, lineY);
	    }
	  }

	  if (j != nColumns - 1 && column[j].lineType != TABLE_LINE_NONE) {
	    // vertical lines
	    if (cell->colSpan <= 1) {
	      scaled lineX = position.x + xOffset + column[j].width + column[j].spacing / 2;
	      scaled lineY = position.y + yOffset;
	      scaled len = row[i].GetHeight();

	      if (i == 0) {
		lineY -= frameVerticalSpacing;
		len   += frameVerticalSpacing;
	      } else {
		lineY -= row[i - 1].spacing / 2;
		len   += row[i - 1].spacing / 2;
	      }
	      if (i == nRows - 1) {
		len   += frameVerticalSpacing;
	      } else {
		len   += row[i].spacing / 2;
	      }

	      area.DrawLine((column[j].lineType == TABLE_LINE_DASHED) ? dGC[Selected()] : fGC[Selected()],
			    lineX, lineY, lineX, lineY + len);
	    }
	  }

	  xOffset += column[j].width + column[j].spacing;
	}

	yOffset += row[i].GetHeight() + row[i].spacing;
      }

      ResetDirty();
    }
}

Ptr<MathMLElement>
MathMLTableElement::Inside(scaled x, scaled y)
{
  if (!IsInside(x, y)) return 0;
  
  for (unsigned i = 0; i < nRows; i++)
    for (unsigned j = 0; j < nColumns; j++)
      if (cell[i][j].mtd && !cell[i][j].spanned) {
	Ptr<MathMLElement> inside = cell[i][j].mtd->Inside(x, y);
	if (inside) return inside;
      }

  return MathMLLinearContainerElement::Inside(x, y);
}

#if 0
// SetDirty: Tables redefine this method for optimization. In fact,
// if the table has no lines it behaves just as a container, and
// only the cells covered by "rect" are effectively rendered again,
// not the whole table.
void
MathMLTableElement::SetDirty(const Rectangle* rect)
{
  bool hasLines = false;

  for (unsigned i = 0; !hasLines && i < nRows; i++)
    hasLines = row[i].lineType != TABLE_LINE_NONE;

  for (unsigned j = 0; !hasLines && j < nColumns; j++)
    hasLines = column[j].lineType != TABLE_LINE_NONE;

  dirtyBackground =
    (GetParent() && (GetParent()->Selected() != Selected())) ? 1 : 0;

  if (IsDirty()) return;
  if (rect != NULL && !GetRectangle().Overlaps(*rect)) return;

  if (hasLines) {
    dirty = 1;
    SetDirtyChildren();
  }

  std::for_each(content.begin(), content.end(),
		std::bind2nd(SetDirtyAdaptor(), rect));
}
#endif

void
MathMLTableElement::ReleaseGCs()
{
  MathMLLinearContainerElement::ReleaseGCs();
  dGC[0] = dGC[1] = NULL;
}
