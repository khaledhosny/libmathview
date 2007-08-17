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

#ifndef __MathMLRadicalElement_hh__
#define __MathMLRadicalElement_hh__

#include "MathMLContainerElement.hh"
#include "BinContainerTemplate.hh"

class GMV_MathView_EXPORT MathMLRadicalElement : public MathMLContainerElement
{
protected:
  MathMLRadicalElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLRadicalElement();

public:
  static SmartPtr<MathMLRadicalElement> create(const SmartPtr<class MathMLNamespaceContext>& view)
  { return new MathMLRadicalElement(view); }

  virtual AreaRef format(class FormattingContext&);
  virtual void setFlagDown(Flags);
  virtual void resetFlagDown(Flags);

  SmartPtr<class MathMLElement> getBase(void) const { return base.getChild(); }
  SmartPtr<class MathMLElement> getIndex(void) const { return index.getChild(); }
  void setBase(const SmartPtr<class MathMLElement>& child) { base.setChild(this, child); }
  void setIndex(const SmartPtr<class MathMLElement>& child) { index.setChild(this, child); }

private:
  BinContainerTemplate<MathMLRadicalElement,MathMLElement> base;
  BinContainerTemplate<MathMLRadicalElement,MathMLElement> index;
};

#endif // __MathMLRadicalElement_hh__
