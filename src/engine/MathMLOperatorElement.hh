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

#ifndef __MathMLOperatorElement_hh__
#define __MathMLOperatorElement_hh__

#include "MathMLEmbellishment.hh"
#include "MathMLTokenElement.hh"
#include "token.hh"

class MathMLOperatorElement
  : public MathMLTokenElement, public MathMLEmbellishment
{
protected:
  MathMLOperatorElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLOperatorElement();

public:
  static SmartPtr<MathMLOperatorElement> create(const SmartPtr<class MathMLNamespaceContext>& view)
  { return new MathMLOperatorElement(view); }

  virtual AreaRef format(class FormattingContext&);

  bool         IsStretchy(void) const { return stretchy; }
  bool         IsAccent(void) const { return accent; }
  bool         ForcedFence(void) const { return forcedFence; }
  bool         IsFence(void) const { return fence; }
  bool         ForcedSeparator(void) const { return forcedSeparator; }
  bool         IsSeparator(void) const { return separator; }
  bool         ForcedSymmetric(void) const { return forcedSymmetric; }
  bool         IsSymmetric(void) const { return symmetric; }
  bool         HasMovableLimits(void) const { return movableLimits; }
  void         SetFence(void) { forcedFence = fence = true; }
  void         SetSeparator(void) { forcedSeparator = separator = true; }
  void         ResetSymmetric(void) { forcedSymmetric = true; symmetric = false; }

  scaled       getLeftPadding(void) const { return lSpace; }
  scaled       getRightPadding(void) const { return rSpace; }

  virtual SmartPtr<MathMLOperatorElement> getCoreOperator(void);

private:
  TokenId inferOperatorForm(void);
  SmartPtr<Value> getOperatorAttributeValue(const struct AttributeSignature&, const SmartPtr<class AttributeSet>&) const;
  void parseLimitValue(const SmartPtr<Value>&, const class FormattingContext&, float&, scaled&);

  bool largeOp;
  bool forcedFence;
  bool fence;
  bool forcedSeparator;
  bool separator;
  bool forcedSymmetric;
  bool symmetric;
  bool stretchy;
  bool movableLimits;
  bool accent;
  scaled lSpace;
  scaled rSpace;
};

#endif // __MathMLOperatorElement_hh__
