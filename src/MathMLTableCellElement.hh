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

#if defined(HAVE_MINIDOM)
#include <minidom.h>
#elif defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLTableElement.hh"
#include "MathMLNormalizingContainerElement.hh"

class MathMLTableCellElement: public MathMLNormalizingContainerElement
{
public:
#if defined(HAVE_MINIDOM)
  MathMLTableCellElement(mDOMNodeRef);
#elif defined(HAVE_GMETADOM)
  MathMLTableCellElement(GMetaDOM::Element&);
#endif
  virtual const AttributeSignature* GetAttributeSignature(AttributeId) const;
  virtual void Setup(RenderingEnvironment*);
  virtual void SetPosition(scaled, scaled);
  virtual ~MathMLTableCellElement();

  void SetAlignmentScope(bool b) { alignmentScope = b; }

  unsigned GetRowIndex(void) const { return rowIndex; }
  unsigned GetColumnIndex(void) const { return columnIndex; }
  unsigned GetRowSpan(void) const { return rowSpan; }
  unsigned GetColumnSpan(void) const { return columnSpan; }
  bool     GetAlignmentScope(void) const { return alignmentScope; }
  bool     IsStretchyOperator(void) const;

  friend class MathMLTableElement;
  friend class MathMLTableRowElement;

protected:
  // the following method is declared static for efficiency reasons. In fact,
  // it does not access any non-static method of the class but it is recursive
  // (and relevant to the table cell)
  static void SetupGroups(MathMLElement*, bool, bool, TableCell&);
  void CalcGroupsExtent(void);

  void SetupCellPosition(unsigned, unsigned, unsigned);
  void SetupCellSpanning(RenderingEnvironment*);
  void SetupCell(TableCell*);

private:
  unsigned rowSpan;
  unsigned columnSpan;

  unsigned rowIndex;
  unsigned columnIndex;
  TableCell* cell;

  bool     alignmentScope;      // TRUE if this cell is within an alignment scope
};

typedef MathMLTableCellElement* MathMLTableCellElementPtr;

#define TO_TABLECELL(object) (dynamic_cast<MathMLTableCellElement*>(object))

#endif // MathMLTableCellElement_hh
