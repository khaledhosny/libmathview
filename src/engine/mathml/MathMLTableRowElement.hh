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

#ifndef MathMLTableRowElement_hh
#define MathMLTableRowElement_hh

#include "MathMLLinearContainerElement.hh"

class MathMLTableRowElement
  : public MathMLLinearContainerElement
{
protected:
  MathMLTableRowElement(const SmartPtr<class MathMLView>&);
  virtual ~MathMLTableRowElement();

public:
  static SmartPtr<MathMLTableRowElement> create(const SmartPtr<class MathMLView>& view)
  { return new MathMLTableRowElement(view); }

  virtual void construct(void);
  virtual void refine(class AbstractRefinementContext&);
#if 0
  virtual void Setup(RenderingEnvironment&);
  
  virtual bool IsInside(const scaled&, const scaled&) const;
#endif

  virtual SmartPtr<MathMLElement> GetLabel(void) const;

  friend class MathMLTableElement;

  virtual void setDirtyStructure(void);
  virtual void setDirtyAttribute(void);
  virtual void setDirtyLayout(void);

protected:
#if 0
  void SetupAux(RenderingEnvironment&, bool);
  void SetupCellSpanning(RenderingEnvironment&);
  void SetupGroupAlign(RenderingEnvironment&);
  void SetupRowIndex(unsigned);
#endif

private:
  unsigned rowIndex;
};

#endif // MathMLTableRowElement_hh
