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

#ifndef MathMLLabeledTableRowElement_hh
#define MathMLLabeledTableRowElement_hh

#include "MathMLTableRowElement.hh"

class MathMLLabeledTableRowElement : public MathMLTableRowElement
{
protected:
  MathMLLabeledTableRowElement(const SmartPtr<class MathMLView>&);
  virtual ~MathMLLabeledTableRowElement();

public:
  static SmartPtr<MathMLLabeledTableRowElement> create(const SmartPtr<class MathMLView>& view)
  { return new MathMLLabeledTableRowElement(view); }

  void SetLabel(const SmartPtr<MathMLElement>&);
  virtual SmartPtr<MathMLElement> GetLabel(void) const;

  virtual void construct(void);
#if 0
  virtual void Setup(class RenderingEnvironment&);
#endif
  virtual void SetFlagDown(Flags);
  virtual void ResetFlagDown(Flags);
#if 0
  virtual SmartPtr<MathMLElement> Inside(const scaled&, const scaled&);
#endif

  friend class MathMLTableElement;

private:
  SmartPtr<MathMLElement> label;
};

#endif // MathMLLabeledTableRowElement_hh
