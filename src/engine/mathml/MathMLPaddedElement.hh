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

#ifndef __MathMLPaddedElement_hh__
#define __MathMLPaddedElement_hh__

#include "MathMLEmbellishment.hh"
#include "MathMLNormalizingContainerElement.hh"

class MathMLPaddedElement
  : public MathMLNormalizingContainerElement, public MathMLEmbellishment
{
protected:
  MathMLPaddedElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLPaddedElement();

public:
  static SmartPtr<MathMLPaddedElement> create(const SmartPtr<class MathMLNamespaceContext>& view)
  { return new MathMLPaddedElement(view); }

  //virtual void refine(class AbstractRefinementContext&);
  virtual SmartPtr<class MathMLOperatorElement> getCoreOperator(void);

private:
  struct LengthDimension
  {
    bool valid;        // true if this attribute was set
    int sign;         // -1 => -, 0 => no sign specified, +1 => +
    float number;       // the (unsigned) number specified
    bool percentage;   // true if % was set (=> pseudo == true)
    bool pseudo;       // true if pseudo unit used
    TokenId pseudoUnitId; // pseudo-unit (valid if pseudo == true)
    scaled unit;         // standard unit (valid if pseudo == false)
  };

#if 0
  void ParseLengthDimension(RenderingEnvironment&, const SmartPtr<Value>&, LengthDimension&, TokenId);
#endif
  scaled EvalLengthDimension(const scaled&, const LengthDimension&, const BoundingBox&) const;

  LengthDimension width;
  LengthDimension lSpace;
  LengthDimension height;
  LengthDimension depth;

  scaled lSpaceE; // evaluated
};

#endif // __MathMLPaddedElement_hh__
