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

#include <config.h>

#include <cassert>

#include <algorithm>
#include <functional>

#include "Adaptors.hh"
#include "ChildList.hh"
#include "MathMLFormattingEngineFactory.hh"
#include "MathMLTableCellElement.hh"
#include "MathMLTableElement.hh"
#include "MathMLView.hh"
#include "defs.h"

MathMLTableElement::MathMLTableElement(const SmartPtr<class MathMLView>& view)
  : MathMLLinearContainerElement(view)
{
  nRows    = 0;
  nColumns = 0;
  cell     = 0;
  column   = 0;
  row      = 0;
  rowLabel = 0;
  width    = 0;

  dGC[0] = dGC[1] = 0;
}

MathMLTableElement::~MathMLTableElement()
{
  ReleaseAuxStructures();
}

void
MathMLTableElement::construct()
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  ChildList children(getDOMElement(), MATHML_NS_URI, "*");
	  unsigned n = children.get_length();

	  std::vector< SmartPtr<MathMLElement> > newContent;
	  newContent.reserve(n);
	  for (unsigned i = 0; i < n; i++)
	    {
	      DOM::Element node = children.item(i);
	      assert(node);
	      if (nodeLocalName(node) == "mtr" || nodeLocalName(node) == "mlabeledtr")
		{
		  SmartPtr<MathMLElement> elem = getFormattingNode(node);
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
	Append(smart_cast<MathMLTableRowElement>(getFactory()->createTableRowElement(getView())));

      std::for_each(content.begin(), content.end(), ConstructAdaptor());

      ResetDirtyStructure();
    }
}

void
MathMLTableElement::refine(AbstractRefinementContext& context)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, Table, align);
      REFINE_ATTRIBUTE(context, Table, rowalign);
      REFINE_ATTRIBUTE(context, Table, columnalign);
      REFINE_ATTRIBUTE(context, Table, groupalign);
      REFINE_ATTRIBUTE(context, Table, alignmentscope);
      REFINE_ATTRIBUTE(context, Table, columnwidth);
      REFINE_ATTRIBUTE(context, Table, rowspacing);
      REFINE_ATTRIBUTE(context, Table, columnspacing);
      REFINE_ATTRIBUTE(context, Table, rowlines);
      REFINE_ATTRIBUTE(context, Table, columnlines);
      REFINE_ATTRIBUTE(context, Table, frame);
      REFINE_ATTRIBUTE(context, Table, framespacing);
      REFINE_ATTRIBUTE(context, Table, equalrows);
      REFINE_ATTRIBUTE(context, Table, equalcolumns);
      REFINE_ATTRIBUTE(context, Table, displaystyle);
      REFINE_ATTRIBUTE(context, Table, side);
      REFINE_ATTRIBUTE(context, Table, minlabelspacing);
      REFINE_ATTRIBUTE(context, Table, width);
      MathMLLinearContainerElement::refine(context);
    }
}

void
MathMLTableElement::SetPosition(const scaled& x, const scaled& y)
{
  position.x = x;
  position.y = y;

  scaled yOffset = frameVerticalSpacing - box.height;

  for (unsigned i = 0; i < nRows; i++) {
    scaled xOffset = frameHorizontalSpacing;

    if (HasLabels()) {
      if (rowLabel[i].labelElement &&
	  (side == T_LEFT || side == T_LEFTOVERLAP))
	  SetLabelPosition(i, x, y + yOffset + row[i].height);
      
      xOffset += leftPadding;
    }

    //printf("row set position %d %d height = %d\n", sp2ipx(x + xOffset), sp2ipx(y + yOffset + row[i].height), sp2ipx(row[i].verticalExtent() + row[i].spacing));

    if (row[i].mtr)
      row[i].mtr->SetPosition(x + xOffset, y + yOffset + row[i].height);

    for (unsigned j = 0; j < nColumns; j++) {
      TableCell* cell = GetCell(i, j);

      if (cell->mtd && !cell->spanned) {
	const BoundingBox& cellBox = cell->mtd->GetBoundingBox();
	//printf("cell set position %d %d\n", sp2ipx(x + xOffset), sp2ipx(y + yOffset + cellBox.height));
	cell->mtd->SetPosition(x + xOffset, y + yOffset + cellBox.height);
      }
      
      xOffset += column[j].width;
      if (j < nColumns - 1) xOffset += column[j].spacing;
    }

    if (HasLabels()) {
      xOffset += frameHorizontalSpacing;

      if (rowLabel[i].labelElement &&
	  (side == T_RIGHT || side == T_RIGHTOVERLAP))
	SetLabelPosition(i, x + xOffset, y + yOffset + row[i].height);
    }

    yOffset += row[i].verticalExtent() + row[i].spacing;
  }
}

// SetLabelPosition:
// if side == LEFT? then x is the left edge of the whole table
// if side == RIGHT? then x is the right edge of the whole table (and thus
// must be adjust to accomodate the label)
void
MathMLTableElement::SetLabelPosition(unsigned i, const scaled& x0, const scaled& y0)
{
  assert(i < nRows);
  assert(rowLabel);
  assert(rowLabel[i].labelElement);

  scaled x = x0;
  scaled y = y0;
  const BoundingBox& labelBox = rowLabel[i].labelElement->GetBoundingBox();

  switch (rowLabel[i].rowAlign) {
  case T_BOTTOM:
    y += row[i].depth - labelBox.depth;
    break;
  case T_CENTER:
    y += (row[i].verticalExtent() - labelBox.verticalExtent()) / 2 +
      labelBox.height - row[i].height;
    break;
  case T_BASELINE:
    break;
  case T_AXIS:
    assert(IMPOSSIBLE);
    break;
  case T_TOP:
    y += labelBox.height - row[i].height;
    break;
  default:
    break;
  }

  scaled columnWidth;
  if (side == T_LEFT || side == T_LEFTOVERLAP) columnWidth = leftPadding;
  else columnWidth = box.width - leftPadding - tableWidth;

  switch (rowLabel[i].columnAlign) {
  case T_RIGHT:
    x += columnWidth - labelBox.width;
    break;
  case T_CENTER:
    x += (columnWidth - labelBox.width) / 2;
    break;
  case T_LEFT:
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
  if (Exposed(area))
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

      if (frame != T_NONE) {
	Rectangle rect;

	if (HasLabels()) {
	  rect.x = GetX() + leftPadding;
	  rect.y = GetY() - box.height;    
	  rect.width = tableWidth;
	  rect.height = box.verticalExtent();
	} else
	  rect = Rectangle(GetX(), GetY(), box);

	area.DrawRectangle((frame == T_DASHED) ? dGC[Selected()] : fGC[Selected()], rect);
      }

      scaled yOffset = frameVerticalSpacing - box.height;
      for (unsigned i = 0; i < nRows; i++) {
	scaled xOffset = frameHorizontalSpacing;
	if (HasLabels()) xOffset += leftPadding;

	for (unsigned j = 0; j < nColumns; j++) {
	  TableCell* cell = GetCell(i, j);

	  if (i != nRows - 1 && row[i].lineType != T_NONE) {
	    // horizontal lines
	    if (cell->rowSpan <= 1) {
	      scaled lineX = position.x + xOffset;
	      scaled lineY = position.y + yOffset + row[i].verticalExtent() + row[i].spacing / 2;
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

	      area.DrawLine((row[i].lineType == T_DASHED) ? dGC[Selected()] : fGC[Selected()],
			    lineX, lineY, lineX + len, lineY);
	    }
	  }

	  if (j != nColumns - 1 && column[j].lineType != T_NONE) {
	    // vertical lines
	    if (cell->colSpan <= 1) {
	      scaled lineX = position.x + xOffset + column[j].width + column[j].spacing / 2;
	      scaled lineY = position.y + yOffset;
	      scaled len = row[i].verticalExtent();

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

	      area.DrawLine((column[j].lineType == T_DASHED) ? dGC[Selected()] : fGC[Selected()],
			    lineX, lineY, lineX, lineY + len);
	    }
	  }

	  xOffset += column[j].width + column[j].spacing;
	}

	yOffset += row[i].verticalExtent() + row[i].spacing;
      }

      ResetDirty();
    }
}

SmartPtr<MathMLElement>
MathMLTableElement::Inside(const scaled& x, const scaled& y)
{
  if (!IsInside(x, y)) return 0;
  
  for (unsigned i = 0; i < nRows; i++)
    for (unsigned j = 0; j < nColumns; j++)
      if (cell[i][j].mtd && !cell[i][j].spanned) {
	SmartPtr<MathMLElement> inside = cell[i][j].mtd->Inside(x, y);
	if (inside) return inside;
      }

  return MathMLLinearContainerElement::Inside(x, y);
}

void
MathMLTableElement::ReleaseGCs()
{
  MathMLLinearContainerElement::ReleaseGCs();
  dGC[0] = dGC[1] = NULL;
}
