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

#ifndef MathMLFractionElement_hh
#define MathMLFractionElement_hh

#include "MathMLEmbellishment.hh"
#include "MathMLContainerElement.hh"

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
  virtual void   Setup(RenderingEnvironment&);
  virtual void   refine(class AbstractRefinementContext&);
  virtual void   DoLayout(const class FormattingContext&);
  virtual void   SetPosition(const scaled&, const scaled&);
  virtual void   SetFlagDown(Flags);
  virtual void   ResetFlagDown(Flags);

  virtual void   Render(const DrawingArea&);

  virtual void   Replace(const SmartPtr<class MathMLElement>&, const SmartPtr<class MathMLElement>&);

  virtual scaled GetLeftEdge(void) const;
  virtual scaled GetRightEdge(void) const;
  virtual void   ReleaseGCs(void);
  virtual SmartPtr<class MathMLElement> Inside(const scaled&, const scaled&);
  virtual SmartPtr<class MathMLOperatorElement> GetCoreOperator(void);

  SmartPtr<MathMLElement> GetNumerator(void) const { return numerator; }
  SmartPtr<MathMLElement> GetDenominator(void) const { return denominator; }
  void SetNumerator(const SmartPtr<MathMLElement>&);
  void SetDenominator(const SmartPtr<MathMLElement>&);

private:
  SmartPtr<MathMLElement> numerator;
  SmartPtr<MathMLElement> denominator;

  scaled          axis;
  scaled          numShift;
  scaled          denomShift;
#ifdef TEXISH_MATHML
  scaled          numMinShift;
  scaled          denomMinShift;
  scaled          defaultRuleThickness;
#else
  scaled          minShift;
#endif // TEXISH_MATHML
  scaled          lineThickness;

  FractionAlignId numAlign;
  FractionAlignId denomAlign;
  bool            displayStyle;
  RGBValue        color;

  bool            bevelled;
};

#endif // MathMLFractionElement_hh
