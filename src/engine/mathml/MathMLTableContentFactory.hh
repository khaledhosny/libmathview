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

#ifndef __MathMLTableContentFactory_hh__
#define __MathMLTableContentFactory_hh__

#include <vector>

#include "MathMLTableCellElement.hh"

class MathMLTableContentFactory
{
public:
  MathMLTableContentFactory(void) { }

  std::pair<unsigned, unsigned> size(void) const;
  unsigned addCell(unsigned, unsigned, const SmartPtr<MathMLTableCellElement>&);
  SmartPtr<MathMLTableCellElement> getCell(unsigned, unsigned) const;

private:    
  struct Cell
  {
    Cell(const SmartPtr<MathMLTableCellElement>& el = 0, bool s = false) : cell(el), spanned(s) { }
    
    bool free(void) const { return !cell; }
    SmartPtr<MathMLTableCellElement> getCell(void) const { return cell; }

  private:
    SmartPtr<MathMLTableCellElement> cell;
    bool spanned;
  };
    
  struct Row
  {
    Row(void) { }
      
    unsigned size(void) const { return cells.size(); }
    unsigned addCell(unsigned, const SmartPtr<MathMLTableCellElement>&);
    unsigned findSlot(unsigned, unsigned);
    void setSpanningCell(unsigned, const SmartPtr<MathMLTableCellElement>&);
    SmartPtr<MathMLTableCellElement> getCell(unsigned) const;

  private:
    std::vector<Cell> cells;
  };

  std::vector<Row> rows;
};

#endif // __MathMLTableContentFactory_hh__
