// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

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

  virtual AreaRef format(class FormattingContext&);
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

  static void parseLengthDimension(const class FormattingContext&,
				   const SmartPtr<Value>&,
				   LengthDimension&, 
				   TokenId);
  static scaled evalLengthDimension(const class FormattingContext&, 
				    const SmartPtr<Value>&, 
				    TokenId, const scaled&,
				    const BoundingBox&);

  LengthDimension width;
  LengthDimension lSpace;
  LengthDimension height;
  LengthDimension depth;

  scaled lSpaceE; // evaluated
};

#endif // __MathMLPaddedElement_hh__
