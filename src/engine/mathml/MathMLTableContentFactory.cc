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

#include "MathMLTableContentFactory.hh"

unsigned
MathMLTableContentFactory::addCell(unsigned i, unsigned j,
				   const SmartPtr<MathMLTableCellElement>& cell)
{
  while (rows.size() <= i) rows.push_back(Row());
  unsigned trueJ = rows[i].addCell(j, cell);
  for (unsigned k = 1; k < cell->getRowSpan(); k++)
    rows[i + k].setSpanningCell(trueJ, cell);
  return trueJ;
}

SmartPtr<MathMLTableCellElement>
MathMLTableContentFactory::getCell(unsigned i, unsigned j) const
{
  return (i < rows.size()) ? rows[i].getCell(j) : 0;
}

std::pair<unsigned, unsigned>
MathMLTableContentFactory::size() const
{
  unsigned maxI = 0;
  for (std::vector<Row>::const_iterator p = rows.begin(); p != rows.end(); p++)
    maxI = std::max(maxI, p->size());
  return std::make_pair(rows.size(), maxI);
}

unsigned
MathMLTableContentFactory::Row::addCell(unsigned j, const SmartPtr<MathMLTableCellElement>& cell)
{
  unsigned n = cell->getColumnSpan();
  j = findSlot(j, n);
  assert(j + n <= cells.size());
  cells[j] = Cell(cell);
  for (unsigned k = 1; k < n; k++)
    cells[j + k] = Cell(cell, true);
  return j;
}

unsigned
MathMLTableContentFactory::Row::findSlot(unsigned j, unsigned columnSpan)
{
  unsigned k = j;
  while (k < cells.size() && k < j + columnSpan)
    {
      if (!cells[k].free()) j = k + 1;
      k++;
    }

  while (k + columnSpan > cells.size()) cells.push_back(Cell());

  return j;
}

void
MathMLTableContentFactory::Row::setSpanningCell(unsigned j, const SmartPtr<MathMLTableCellElement>& cell)
{
  unsigned n = cell->getColumnSpan();
  assert(j + n <= cells.size());
  for (unsigned k = 0; k < n; k++)
    cells[j + k] = Cell(cell, true);
}

SmartPtr<MathMLTableCellElement>
MathMLTableContentFactory::Row::getCell(unsigned j) const
{
  return (j < cells.size()) ? cells[j].getCell() : 0;
}
