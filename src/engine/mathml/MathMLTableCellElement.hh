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

#ifndef MathMLTableCellElement_hh
#define MathMLTableCellElement_hh

#include "MathMLNormalizingContainerElement.hh"

class MathMLTableCellElement : public MathMLNormalizingContainerElement
{
protected:
  MathMLTableCellElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLTableCellElement();

public:
  static SmartPtr<MathMLTableCellElement> create(const SmartPtr<class MathMLNamespaceContext>& view)
  { return new MathMLTableCellElement(view); }

  void SetAlignmentScope(bool b) { alignmentScope = b; }

  scaled   GetMinWidth(void) const { return minWidth; }
  bool     GetAlignmentScope(void) const { return alignmentScope; }
  bool     IsStretchyOperator(void) const;

  friend class MathMLTableElement;
  friend class MathMLTableRowElement;

  virtual void setDirtyAttribute(void);
  virtual void setDirtyStructure(void);
  virtual void setDirtyLayout(void);

  unsigned getRowSpan(void) const { return rowSpan; }
  unsigned getColumnSpan(void) const { return columnSpan; }
  void setSpan(unsigned, unsigned);
  void setPosition(unsigned, unsigned);
  void setAlignment(const SmartPtr<Value>&, const SmartPtr<Value>&, const SmartPtr<Value>&);

protected:
  // the following method is declared static for efficiency reasons. In fact,
  // it does not access any non-static method of the class but it is recursive
  // (and relevant to the table cell)
  static void SetupGroups(const SmartPtr<MathMLElement>&, bool, bool, class TableCell&);
  void CalcGroupsExtent(void);

#if 0
  void SetupCellPosition(unsigned, unsigned, unsigned);
  void SetupCellSpanning(RenderingEnvironment&);
  void SetupCell(class TableCell*);
#endif

private:
  unsigned rowSpan;
  unsigned columnSpan;

  scaled   minWidth;
  unsigned rowIndex;
  unsigned columnIndex;
  class TableCell* cell;

  bool     alignmentScope;      // TRUE if this cell is within an alignment scope
};

#endif // MathMLTableCellElement_hh
