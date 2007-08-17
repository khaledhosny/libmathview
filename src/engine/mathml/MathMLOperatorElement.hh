// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __MathMLOperatorElement_hh__
#define __MathMLOperatorElement_hh__

#include "MathMLEmbellishment.hh"
#include "MathMLTokenElement.hh"
#include "token.hh"

class GMV_MathView_EXPORT MathMLOperatorElement
  : public MathMLTokenElement, public MathMLEmbellishment
{
protected:
  MathMLOperatorElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLOperatorElement();

public:
  static SmartPtr<MathMLOperatorElement> create(const SmartPtr<class MathMLNamespaceContext>& view)
  { return new MathMLOperatorElement(view); }

  virtual AreaRef format(class FormattingContext&);

  bool         IsStretchy(void) const { return stretchy != 0; }
  bool         IsAccent(void) const { return accent != 0; }
  bool         ForcedFence(void) const { return forcedFence != 0; }
  bool         IsFence(void) const { return fence != 0; }
  bool         ForcedSeparator(void) const { return forcedSeparator != 0; }
  bool         IsSeparator(void) const { return separator != 0; }
  bool         ForcedSymmetric(void) const { return forcedSymmetric != 0; }
  bool         IsSymmetric(void) const { return symmetric != 0; }
  bool         HasInfiniteMaxSize(void) const { return infiniteMaxSize != 0; }
  bool         HasMovableLimits(void) const { return movableLimits != 0; }
  void         SetFence(void) { forcedFence = fence = 1; }
  void         SetSeparator(void) { forcedSeparator = separator = 1; }
  void         ResetSymmetric(void) { forcedSymmetric = 1; symmetric = 0; }

  scaled       getLeftPadding(void) const { return lSpace; }
  scaled       getRightPadding(void) const { return rSpace; }

  virtual SmartPtr<MathMLOperatorElement> getCoreOperator(void);

private:
  TokenId inferOperatorForm(void);
  SmartPtr<Value> getOperatorAttributeValue(const class AttributeSignature&, const SmartPtr<class AttributeSet>&) const;
  void parseLimitValue(const SmartPtr<Value>&, const class FormattingContext&, float&, scaled&);

  unsigned largeOp : 1;
  unsigned forcedFence : 1;
  unsigned fence : 1;
  unsigned forcedSeparator : 1;
  unsigned separator : 1;
  unsigned forcedSymmetric : 1;
  unsigned symmetric : 1;
  unsigned stretchy : 1;
  unsigned infiniteMaxSize : 1; // if != 0 => no max size contraint
  unsigned movableLimits : 1;
  unsigned accent : 1;
  scaled lSpace;
  scaled rSpace;
};

#endif // __MathMLOperatorElement_hh__
