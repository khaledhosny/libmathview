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

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLLinearContainerElement.hh"

class MathMLTableRowElement
  : public MathMLLinearContainerElement
{
protected:
  MathMLTableRowElement(void);
#if defined(HAVE_GMETADOM)
  MathMLTableRowElement(const DOM::Element&);
#endif
  virtual ~MathMLTableRowElement();

public:
  static Ptr<MathMLElement> create(void)
  { return Ptr<MathMLElement>(new MathMLTableRowElement()); }
#if defined(HAVE_GMETADOM)
  static Ptr<MathMLElement> create(const DOM::Element& el)
  { return Ptr<MathMLElement>(new MathMLTableRowElement(el)); }
#endif

  virtual const AttributeSignature* GetAttributeSignature(AttributeId) const;
  virtual void Normalize(const Ptr<class MathMLDocument>&);
  virtual void Setup(RenderingEnvironment&);
  
  //virtual void SetDirty(const Rectangle*);
  virtual bool IsInside(scaled, scaled) const;

  virtual Ptr<MathMLElement> GetLabel(void) const;

  friend class MathMLTableElement;

  virtual void SetDirtyStructure(void);
  virtual void SetDirtyAttribute(void);
  virtual void SetDirtyLayout(void);

protected:
  void SetupAux(RenderingEnvironment&, bool);
  void SetupCellSpanning(RenderingEnvironment&);
  void SetupGroupAlign(RenderingEnvironment&);
  void SetupRowIndex(unsigned);

private:
  unsigned rowIndex;
};

#endif // MathMLTableRowElement_hh
