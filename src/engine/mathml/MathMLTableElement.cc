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
  assert(cellContent.size() % labelContent.size() == 0);
  numRows = labelContent.size();
  numColumns = cellContent.size() / numRows;
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
      std::cerr << "formatting table 1" << std::endl;
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
			       GET_ATTRIBUTE_VALUE(MathML, Table, equalcolumns),
			       GET_ATTRIBUTE_VALUE(MathML, Table, side),
			       GET_ATTRIBUTE_VALUE(MathML, Table, minlabelspacing));
	}

      std::cerr << "formatting table 2" << std::endl;
      for_each_if(cell.begin(), cell.end(),
		  std::bind2nd(FormatAdapter<FormattingContext,MathMLTableCellElement,AreaRef>(), &ctxt),
		  NotNullPredicate<MathMLTableCellElement>());
#if 0
      // questo for_each fallisce ma non si capisce perche' :(
      for_each_if(label.begin(), label.end(),
		  std::bind2nd(FormatAdapter<FormattingContext,MathMLTableCellElement,AreaRef>(), &ctxt),
		  NotNullPredicate<MathMLTableCellElement>());
#endif
      std::cerr << "formatting table 2 bis" << std::endl;

      SmartPtr<Value> v1 = GET_ATTRIBUTE_VALUE(MathML, Table, align);
      SmartPtr<Value> v2 = GET_ATTRIBUTE_VALUE(MathML, Table, equalrows);
      SmartPtr<Value> v3 = GET_ATTRIBUTE_VALUE(MathML, Table, equalcolumns);
      AreaRef res = tableFormatter->format(ctxt,
					   numRows,
					   v1,
					   v2,
					   v3);
      std::cerr << "formatting table 3" << std::endl;
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
