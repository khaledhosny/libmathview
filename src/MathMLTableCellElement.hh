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

#ifndef MathMLTableCellElement_hh
#define MathMLTableCellElement_hh

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLNormalizingContainerElement.hh"

class MathMLTableCellElement: public MathMLNormalizingContainerElement
{
protected:
  MathMLTableCellElement(void);
#if defined(HAVE_GMETADOM)
  MathMLTableCellElement(const DOM::Element&);
#endif
  virtual ~MathMLTableCellElement();

private:
  void Init(void);

public:
  static Ptr<MathMLElement> create(void)
  { return Ptr<MathMLElement>(new MathMLTableCellElement()); }
#if defined(HAVE_GMETADOM)
  static Ptr<MathMLElement> create(const DOM::Element& el)
  { return Ptr<MathMLElement>(new MathMLTableCellElement(el)); }
#endif

  virtual const AttributeSignature* GetAttributeSignature(AttributeId) const;
  virtual void Setup(RenderingEnvironment&);
  virtual void SetPosition(scaled, scaled);
  virtual void DoLayout(const class FormattingContext&);

  void SetAlignmentScope(bool b) { alignmentScope = b; }

  scaled   GetMinWidth(void) const { return minWidth; }
  unsigned GetRowIndex(void) const { return rowIndex; }
  unsigned GetColumnIndex(void) const { return columnIndex; }
  unsigned GetRowSpan(void) const { return rowSpan; }
  unsigned GetColumnSpan(void) const { return columnSpan; }
  bool     GetAlignmentScope(void) const { return alignmentScope; }
  bool     IsStretchyOperator(void) const;

  friend class MathMLTableElement;
  friend class MathMLTableRowElement;

  virtual void SetDirtyAttribute(void);
  virtual void SetDirtyStructure(void);
  virtual void SetDirtyLayout(void);

protected:
  // the following method is declared static for efficiency reasons. In fact,
  // it does not access any non-static method of the class but it is recursive
  // (and relevant to the table cell)
  static void SetupGroups(const Ptr<MathMLElement>&, bool, bool, class TableCell&);
  void CalcGroupsExtent(void);

  void SetupCellPosition(unsigned, unsigned, unsigned);
  void SetupCellSpanning(RenderingEnvironment&);
  void SetupCell(class TableCell*);

private:
  scaled   minWidth;
  unsigned rowSpan;
  unsigned columnSpan;

  unsigned rowIndex;
  unsigned columnIndex;
  class TableCell* cell;

  bool     alignmentScope;      // TRUE if this cell is within an alignment scope
};

#endif // MathMLTableCellElement_hh
