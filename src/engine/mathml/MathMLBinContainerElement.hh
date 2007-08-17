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

#ifndef __MathMLBinContainerElement_hh__
#define __MathMLBinContainerElement_hh__

#include "MathMLContainerElement.hh"
#include "BinContainerTemplate.hh"

// base class for every non-empty MathML container element
class GMV_MathView_EXPORT MathMLBinContainerElement : public MathMLContainerElement
{
protected:
  MathMLBinContainerElement(const SmartPtr<class MathMLNamespaceContext>& view);
  virtual ~MathMLBinContainerElement();

public:
  virtual AreaRef format(class FormattingContext&);

  SmartPtr<MathMLElement> getChild(void) const { return content.getChild(); }
  void setChild(const SmartPtr<MathMLElement>& child) { content.setChild(this, child); }

  virtual void setFlagDown(Flags);
  virtual void resetFlagDown(Flags);

protected:
  BinContainerTemplate<MathMLBinContainerElement,MathMLElement> content;
};

#endif // __MathMLContainerElement_hh__
