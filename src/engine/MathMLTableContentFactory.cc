// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#include <config.h>

#include <cassert>

#include "MathMLTableContentFactory.hh"

unsigned
MathMLTableContentFactory::findCell(unsigned i, unsigned j, unsigned rowSpan, unsigned columnSpan) const
{
  unsigned k;
  unsigned kMax;

  do
    {
      k = kMax = j;
      for (unsigned l = i; l < rows.size() && l < i + rowSpan; l++)
	kMax = std::max(kMax, rows[l].findCell(j, columnSpan));

      if (kMax > k) j = kMax;
    }
  while (kMax > k);

  return j;
}

void
MathMLTableContentFactory::initCell(unsigned i, unsigned j, unsigned rowSpan, unsigned columnSpan)
{
  while (rows.size() < i + rowSpan)
    rows.push_back(Row());

  for (unsigned k = i; k < i + rowSpan; k++)
    rows[k].initCell(j, columnSpan);
}

#include <iostream>

unsigned
MathMLTableContentFactory::setChild(unsigned i, unsigned j,
				    unsigned rowSpan, unsigned columnSpan,
				    const SmartPtr<MathMLTableCellElement>& child)
{
  //std::cerr << "MathMLTableContentFactory::setChild " << i << " " << j << " " << rowSpan << " " << columnSpan << std::endl;
  j = findCell(i, j, rowSpan, columnSpan);
  initCell(i, j, rowSpan, columnSpan);
  rows[i].setChild(j, columnSpan, child);
  for (unsigned k = i + 1; k < i + rowSpan; k++)
    rows[k].setSpanningChild(j, columnSpan, child);
  return j;
}

SmartPtr<MathMLTableCellElement>
MathMLTableContentFactory::getChild(unsigned i, unsigned j) const
{ return (i < rows.size()) ? rows[i].getChild(j) : nullptr; }

void
MathMLTableContentFactory::setLabelChild(unsigned i,
					 const SmartPtr<MathMLTableCellElement>& child)
{
  initCell(i, 0, 1, 0);
  rows[i].setLabelChild(child);
}

SmartPtr<MathMLTableCellElement>
MathMLTableContentFactory::getLabelChild(unsigned i) const
{ return (i < rows.size()) ? rows[i].getLabelChild() : nullptr; }

void
MathMLTableContentFactory::getContent(std::vector<SmartPtr<MathMLTableCellElement> >& cells,
				      std::vector<SmartPtr<MathMLTableCellElement> >& labels,
				      unsigned& numRows, unsigned& numColumns) const
{
  numRows = rows.size();
  numColumns = 0;
  for (const auto & elem : rows)
    numColumns = std::max(numColumns, elem.getSize());

  cells.clear();
  labels.clear();
  cells.reserve(numRows * numColumns);
  labels.reserve(numRows);

  for (unsigned i = 0; i < numRows; i++)
    {
      labels.push_back(rows[i].getLabelChild());
      for (unsigned j = 0; j < numColumns; j++)
	cells.push_back(getChild(i, j));
    }
}

unsigned
MathMLTableContentFactory::Row::findCell(unsigned j, unsigned columnSpan) const
{
  unsigned k = j;
  while (k < content.size() && k < j + columnSpan)
    {
      if (!content[k].free()) j = k + 1;
      k++;
    }

  return j;
}

void
MathMLTableContentFactory::Row::initCell(unsigned j, unsigned columnSpan)
{
  while (content.size() < j)
    content.push_back(Slot());

  for (unsigned k = j; k < j + columnSpan; k++)
    if (k < content.size())
      content[k].init();
    else
      content.push_back(Slot());
}

void
MathMLTableContentFactory::Row::setChild(unsigned j, unsigned columnSpan, const SmartPtr<MathMLTableCellElement>& child)
{
  assert(j + columnSpan <= content.size());
  content[j].init(child, false);
  setSpanningChild(j + 1, columnSpan - 1, child);
}

void
MathMLTableContentFactory::Row::setSpanningChild(unsigned j, unsigned columnSpan, const SmartPtr<MathMLTableCellElement>& child)
{
  for (unsigned k = j; k < j + columnSpan; k++)
    content[k].init(child, true);
}

SmartPtr<MathMLTableCellElement>
MathMLTableContentFactory::Row::getChild(unsigned j) const
{ return (j < content.size()) ? content[j].getChild() : nullptr; }

void
MathMLTableContentFactory::Row::setLabelChild(const SmartPtr<MathMLTableCellElement>& child)
{ labelChild = child; }

SmartPtr<MathMLTableCellElement>
MathMLTableContentFactory::Row::getLabelChild() const
{ return labelChild; }

void
MathMLTableContentFactory::getSize(unsigned& nRows, unsigned& nColumns) const
{
  nRows = rows.size();
  nColumns = 0;
  for (const auto & elem : rows)
    nColumns = std::max(nColumns, elem.getSize());
}
