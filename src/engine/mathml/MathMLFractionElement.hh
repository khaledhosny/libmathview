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

#ifndef __MathMLFractionElement_hh__
#define __MathMLFractionElement_hh__

#include "MathMLEmbellishment.hh"
#include "MathMLContainerElement.hh"
#include "BinContainerTemplate.hh"

class MathMLFractionElement
  : public MathMLContainerElement, public MathMLEmbellishment
{
protected:
  MathMLFractionElement(const SmartPtr<class MathMLView>&);
  virtual ~MathMLFractionElement();

public:
  static SmartPtr<MathMLFractionElement> create(const SmartPtr<class MathMLView>& view)
  { return new MathMLFractionElement(view); }

  virtual void   construct(void);
  virtual void   refine(class AbstractRefinementContext&);
  virtual AreaRef format(class MathFormattingContext&);

  virtual void   setFlagDown(Flags);
  virtual void   resetFlagDown(Flags);

  virtual SmartPtr<class MathMLOperatorElement> GetCoreOperator(void);

  SmartPtr<MathMLElement> getNumerator(void) const { return numerator.getChild(); }
  SmartPtr<MathMLElement> getDenominator(void) const { return denominator.getChild(); }
  void getNumerator(const SmartPtr<MathMLElement>& child) { numerator.setChild(this, child); }
  void getDenominator(const SmartPtr<MathMLElement>&) { denominator.setChild(this, child); }

private:
  BinContainerTemplate<MathMLBinContainerElement, SmartPtr<MathMLElement> > numerator;
  BinContainerTemplate<MathMLBinContainerElement, SmartPtr<MathMLElement> > denominator;
};

#endif // __MathMLFractionElement_hh__
