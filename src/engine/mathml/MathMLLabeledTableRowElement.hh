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

#ifndef __MathMLLabeledTableRowElement_hh__
#define __MathMLLabeledTableRowElement_hh__

#include "MathMLTableRowElement.hh"
#include "BinContainerTemplate.hh"

class MathMLLabeledTableRowElement : public MathMLTableRowElement
{
protected:
  MathMLLabeledTableRowElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLLabeledTableRowElement();

public:
  static SmartPtr<MathMLLabeledTableRowElement> create(const SmartPtr<class MathMLNamespaceContext>& view)
  { return new MathMLLabeledTableRowElement(view); }

  void setLabel(const SmartPtr<MathMLElement>& child) { label.setChild(this, child); }
  virtual SmartPtr<MathMLElement> getLabel(void) const { return label.getChild(); }

  //virtual void construct(void);
  virtual void setFlagDown(Flags);
  virtual void resetFlagDown(Flags);

  friend class MathMLTableElement;

private:
  BinContainerTemplate<MathMLLabeledTableRowElement,MathMLElement> label;
};

#endif // __MathMLLabeledTableRowElement_hh__
