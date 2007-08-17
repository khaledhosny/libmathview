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

#include <cassert>

#include "RGBColor.hh"
#include "MathMLTableCellElement.hh"
#include "MathMLTableElement.hh"
#include "MathMLNamespaceContext.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "MathMLAttributeSignatures.hh"
#include "MathMLTableContentFactory.hh"
#include "defs.h"

MathMLTableElement::MathMLTableElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLContainerElement(context)
{ }

MathMLTableElement::~MathMLTableElement()
{ }

void
MathMLTableElement::setSize(unsigned nr, unsigned nc)
{
  numRows = nr;
  numColumns = nc;
  label.setSize(this, numRows);
  cell.setSize(this, numRows * numColumns);
}

void
MathMLTableElement::getSize(unsigned& nr, unsigned& nc) const
{
  nr = numRows;
  nc = numColumns;
}

void
MathMLTableElement::updateContent(const MathMLTableContentFactory& factory)
{
  unsigned nRows;
  unsigned nColumns;
  factory.getSize(nRows, nColumns);

  std::vector<SmartPtr<MathMLTableCellElement> > cellContent;
  std::vector<SmartPtr<MathMLTableCellElement> > labelContent;

  cellContent.reserve(nRows * nColumns);
  labelContent.reserve(nRows);
  for (unsigned i = 0; i < nRows; i++)
    {
      labelContent.push_back(factory.getLabelChild(i));
      for (unsigned j = 0; j < nColumns; j++)
	cellContent.push_back(factory.getChild(i, j));
    }
     
  swapContent(cellContent, labelContent);
}

void
MathMLTableElement::swapContent(std::vector<SmartPtr<MathMLTableCellElement> >& cellContent,
				std::vector<SmartPtr<MathMLTableCellElement> >& labelContent)
{
  assert((cellContent.size() == 0 && labelContent.size() == 0) || (cellContent.size() % labelContent.size() == 0));
  numRows = labelContent.size();
  numColumns = (numRows > 0) ? (cellContent.size() / numRows) : 0;
  cell.swapContent(this, cellContent);
  label.swapContent(this, labelContent);
}

#include <iostream>
#include "MathMLAttributeParsers.hh"

AreaRef
MathMLTableElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);
      //std::cerr << "formatting table 1" << std::endl;
      if (!tableFormatter)
	{
	  tableFormatter = MathMLTableFormatter::create();

	  tableFormatter->init(ctxt,
			       numRows,
			       numColumns,
			       cell.getContent(),
			       label.getContent(),
			       GET_ATTRIBUTE_VALUE(MathML, Table, columnwidth),
			       GET_ATTRIBUTE_VALUE(MathML, Table, rowspacing),
			       GET_ATTRIBUTE_VALUE(MathML, Table, columnspacing),
			       GET_ATTRIBUTE_VALUE(MathML, Table, frame),
			       GET_ATTRIBUTE_VALUE(MathML, Table, framespacing),
			       GET_ATTRIBUTE_VALUE(MathML, Table, equalrows),
			       GET_ATTRIBUTE_VALUE(MathML, Table, equalcolumns),
			       GET_ATTRIBUTE_VALUE(MathML, Table, side),
			       GET_ATTRIBUTE_VALUE(MathML, Table, minlabelspacing), 
			       GET_ATTRIBUTE_VALUE(MathML, Table, align));
	}

      //std::cerr << "formatting table 2" << std::endl;
      if (SmartPtr<Value> displayStyleV = GET_ATTRIBUTE_VALUE(MathML, Table, displaystyle))
	ctxt.setDisplayStyle(ToBoolean(displayStyleV));

      for_each_if(cell.begin(), cell.end(),
		  NotNullPredicate<MathMLTableCellElement>(),
		  std::bind2nd(FormatAdapter<FormattingContext,MathMLTableCellElement,AreaRef>(), &ctxt));
      for_each_if(label.begin(), label.end(),
		  NotNullPredicate<MathMLTableCellElement>(),
		  std::bind2nd(FormatAdapter<FormattingContext,MathMLTableCellElement,AreaRef>(), &ctxt));
      //std::cerr << "formatting table 2 bis" << std::endl;

      std::vector<BoxedLayoutArea::XYArea> content;
      const BoundingBox tableBox = tableFormatter->format(content);
      AreaRef res = ctxt.MGD()->getFactory()->boxedLayout(tableBox, content);

      if (AreaRef lines = tableFormatter->formatLines(ctxt,
						      GET_ATTRIBUTE_VALUE(MathML, Table, frame),
						      GET_ATTRIBUTE_VALUE(MathML, Table, rowlines),
						      GET_ATTRIBUTE_VALUE(MathML, Table, columnlines)))
	{
	  std::vector<AreaRef> content;
	  content.reserve(2);
	  content.push_back(res);
	  content.push_back(lines);
	  res = ctxt.MGD()->getFactory()->overlapArray(content);
	}
      //std::cerr << "formatting table 3" << std::endl;
      res = ctxt.MGD()->wrapper(ctxt, res);
      setArea(res);
      
      ctxt.pop();

      resetDirtyLayout();
    }

  return getArea();
}

void
MathMLTableElement::setFlagDown(Flags f)
{
  MathMLContainerElement::setFlagDown(f);
  cell.setFlagDown(f);
  label.setFlagDown(f);
}

void
MathMLTableElement::resetFlagDown(Flags f)
{
  MathMLContainerElement::resetFlagDown(f);
  cell.resetFlagDown(f);
  label.resetFlagDown(f);
}

void
MathMLTableElement::setDirtyAttribute()
{
  MathMLContainerElement::setDirtyAttribute();
  invalidateFormatter();
}

void
MathMLTableElement::setDirtyAttributeD()
{
  MathMLContainerElement::setDirtyAttributeD();
  invalidateFormatter();
}

void
MathMLTableElement::invalidateFormatter()
{ tableFormatter = 0; }
