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

#ifndef __MathMLTableContentFactory_hh__
#define __MathMLTableContentFactory_hh__

#include <vector>

#include "MathMLTableCellElement.hh"

class GMV_MathView_EXPORT MathMLTableContentFactory
{
public:
  MathMLTableContentFactory(void) { }

  unsigned setChild(unsigned, unsigned, unsigned, unsigned, const SmartPtr<MathMLTableCellElement>&);
  SmartPtr<MathMLTableCellElement> getChild(unsigned, unsigned) const;
  void setLabelChild(unsigned, const SmartPtr<MathMLTableCellElement>&);
  SmartPtr<MathMLTableCellElement> getLabelChild(unsigned) const;
  void getSize(unsigned&, unsigned&) const;
  void getContent(std::vector<SmartPtr<MathMLTableCellElement> >&,
		  std::vector<SmartPtr<MathMLTableCellElement> >&,
		  unsigned&, unsigned&) const;
  bool hasLabels(void) const;

protected:
  unsigned findCell(unsigned, unsigned, unsigned, unsigned) const;
  void initCell(unsigned, unsigned, unsigned, unsigned);

private:    
  struct Slot
  {
    Slot(const SmartPtr<MathMLTableCellElement>& el = 0, bool s = false)
      : child(el), spanned(s) { }

    void init(const SmartPtr<MathMLTableCellElement>& el = 0, bool s = false)
    { child = el; spanned = s; }

    bool free(void) const { return !child; }
    SmartPtr<MathMLTableCellElement> getChild(void) const
    { return !spanned ? child : 0; }

  private:
    SmartPtr<MathMLTableCellElement> child;
    bool spanned;
  };
    
  struct Row
  {
    Row(void) { }
      
    unsigned findCell(unsigned, unsigned) const;
    void initCell(unsigned, unsigned);
    void setChild(unsigned, unsigned, const SmartPtr<MathMLTableCellElement>&);
    void setSpanningChild(unsigned, unsigned, const SmartPtr<MathMLTableCellElement>&);
    SmartPtr<MathMLTableCellElement> getChild(unsigned) const;
    void setLabelChild(const SmartPtr<MathMLTableCellElement>&);
    SmartPtr<MathMLTableCellElement> getLabelChild(void) const;
    unsigned getSize(void) const { return content.size(); }

  private:
    SmartPtr<MathMLTableCellElement> labelChild;
    std::vector<Slot> content;
  };

  std::vector<Row> rows;
};

#endif // __MathMLTableContentFactory_hh__
