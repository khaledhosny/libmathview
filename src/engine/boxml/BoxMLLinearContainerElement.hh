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

#ifndef __BoxMLLinearContainerElement_hh__
#define __BoxMLLinearContainerElement_hh__

#include "BoxMLElement.hh"
#include "LinearContainerTemplate.hh"

class GMV_MathView_EXPORT BoxMLLinearContainerElement : public BoxMLElement
{
protected:
  BoxMLLinearContainerElement(const SmartPtr<class BoxMLNamespaceContext>&);
  virtual ~BoxMLLinearContainerElement();

public:
  unsigned getSize(void) const { return content.getSize(); }
  void setSize(unsigned size) { content.setSize(this, size); }

  SmartPtr<BoxMLElement> getChild(unsigned i) const { return content.getChild(i); }
  void setChild(unsigned i, const SmartPtr<BoxMLElement>& child) { content.setChild(this, i, child); }
  void appendChild(const SmartPtr<BoxMLElement>& child) { content.appendChild(this, child); }
  void swapContent(std::vector<SmartPtr<BoxMLElement> >& newContent) { content.swapContent(this, newContent); }

  virtual void setFlagDown(Flags f)
  {
    BoxMLElement::setFlagDown(f);
    content.setFlagDown(f);
  }

  virtual void resetFlagDown(Flags f)
  {
    BoxMLElement::resetFlagDown(f);
    content.resetFlagDown(f);
  }

protected:
  LinearContainerTemplate<BoxMLLinearContainerElement,BoxMLElement> content;
};

#endif // __BoxMLLinearContainerElement_hh__
