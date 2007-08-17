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

#ifndef __MathMLElement_hh__
#define __MathMLElement_hh__

#include "Area.hh"
#include "Element.hh"

// MathMLElement: base class for every MathML Element
class GMV_MathView_EXPORT MathMLElement : public Element
{
protected:
  MathMLElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLElement();

public:
  virtual bool IsSpaceLike(void) const;

  virtual SmartPtr<class MathMLOperatorElement> getCoreOperator(void);
  SmartPtr<class MathMLOperatorElement> getCoreOperatorTop(void);

  SmartPtr<class MathMLNamespaceContext> getMathMLNamespaceContext(void) const;
};

#endif // __MathMLElement_hh__
