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

#ifndef MathMLTableRowElement_hh
#define MathMLTableRowElement_hh

#include "MathMLContainerElement.hh"

class MathMLTableRowElement: public MathMLContainerElement {
public:
  MathMLTableRowElement(GdomeElement*, TagId = TAG_MTR);
  virtual const AttributeSignature* GetAttributeSignature(AttributeId) const;
  virtual void Normalize(void);
  virtual void Setup(RenderingEnvironment*);
  virtual ~MathMLTableRowElement();

  virtual void SetDirty(const Rectangle*);
  virtual bool IsInside(scaled, scaled) const;

  MathMLElement* GetLabel(void) const;

  friend class MathMLTableElement;

protected:
  void SetupCellSpanning(RenderingEnvironment*);
  void SetupGroupAlign(RenderingEnvironment*);
  void SetupRowIndex(unsigned);

private:
  unsigned rowIndex;
};

#define TO_TABLEROW(object) (dynamic_cast<MathMLTableRowElement*>(object))

#endif // MathMLTableRowElement_hh
