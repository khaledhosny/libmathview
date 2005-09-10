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

#ifndef __MathMLTableElement_hh__
#define __MathMLTableElement_hh__

#include "MathMLContainerElement.hh"
#include "LinearContainerTemplate.hh"
#include "MathMLTableFormatter.hh"

class GMV_MathView_EXPORT MathMLTableElement : public MathMLContainerElement
{
protected:
  MathMLTableElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLTableElement();

public:
  static SmartPtr<MathMLTableElement> create(const SmartPtr<class MathMLNamespaceContext>& view)
  { return new MathMLTableElement(view); }

  virtual void setFlagDown(Flags);
  virtual void resetFlagDown(Flags);
  virtual void setDirtyAttribute(void);
  virtual void setDirtyAttributeD(void);
  virtual AreaRef format(class FormattingContext&);

  void setSize(unsigned, unsigned);
  void getSize(unsigned&, unsigned&) const;
  void setCell(unsigned i, unsigned j, const SmartPtr<MathMLTableCellElement>& child)
  { cell.setChild(this, i * numColumns + j, child); }
  SmartPtr<MathMLTableCellElement> getChild(unsigned i, unsigned j) const
  { return cell.getChild(i * numColumns + j); }
  void setLabel(unsigned i, const SmartPtr<MathMLTableCellElement>& child)
  { label.setChild(this, i, child); }
  SmartPtr<MathMLTableCellElement> getLabel(unsigned i) const { return label.getChild(i); }
  void updateContent(const class MathMLTableContentFactory&);

protected:
  void invalidateFormatter(void);
  void swapContent(std::vector<SmartPtr<MathMLTableCellElement> >&,
		   std::vector<SmartPtr<MathMLTableCellElement> >&);

private:
  LinearContainerTemplate<MathMLTableElement, MathMLTableCellElement> cell;
  LinearContainerTemplate<MathMLTableElement, MathMLTableCellElement> label;
  unsigned numRows;
  unsigned numColumns;
  SmartPtr<MathMLTableFormatter> tableFormatter;
};

#endif // __MathMLTableElement_hh__
