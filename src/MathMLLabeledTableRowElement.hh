// Copyright (C) 2000-2002, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://www.cs.unibo.it/helm/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef MathMLLabeledTableRowElement_hh
#define MathMLLabeledTableRowElement_hh

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLTableRowElement.hh"

class MathMLLabeledTableRowElement: public MathMLTableRowElement
{
protected:
  MathMLLabeledTableRowElement(void);
#if defined(HAVE_GMETADOM)
  MathMLLabeledTableRowElement(const DOM::Element&);
#endif
  virtual ~MathMLLabeledTableRowElement();

public:
  static Ptr<MathMLElement> create(void)
  { return Ptr<MathMLElement>(new MathMLLabeledTableRowElement()); }
#if defined(HAVE_GMETADOM)
  static Ptr<MathMLElement> create(const DOM::Element& el)
  { return Ptr<MathMLElement>(new MathMLLabeledTableRowElement(el)); }
#endif

  void SetLabel(const Ptr<MathMLElement>&);
  virtual Ptr<MathMLElement> GetLabel(void) const;

  virtual void Normalize(const Ptr<class MathMLDocument>&);
  virtual void Setup(class RenderingEnvironment&);
  //virtual void SetDirty(const Rectangle*);
  virtual void SetFlagDown(Flags);
  virtual void ResetFlagDown(Flags);
  virtual Ptr<MathMLElement> Inside(scaled, scaled);

  friend class MathMLTableElement;

private:
  Ptr<MathMLElement> label;
};

#endif // MathMLLabeledTableRowElement_hh
