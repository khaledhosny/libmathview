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

#ifndef __MathMLBoxMLAdapter_hh__
#define __MathMLBoxMLAdapter_hh__

#include "BoxMLElement.hh"
#include "MathMLElement.hh"
#include "BinContainerTemplate.hh"

class MathMLBoxMLAdapter : public MathMLElement
{
protected:
  MathMLBoxMLAdapter(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLBoxMLAdapter();

public:
  static SmartPtr<MathMLBoxMLAdapter> create(const SmartPtr<class MathMLNamespaceContext>&);

  virtual AreaRef format(class FormattingContext&);

  SmartPtr<BoxMLElement> getChild(void) const { return content.getChild(); }
  void setChild(const SmartPtr<BoxMLElement>& child) { content.setChild(this, child); }

  virtual void setFlagDown(Flags f)
  {
    MathMLElement::setFlagDown(f);
    content.setFlagDown(f);
  }

  virtual void resetFlagDown(Flags f)
  {
    MathMLElement::resetFlagDown(f);
    content.resetFlagDown(f);
  }

private:
  BinContainerTemplate<MathMLBoxMLAdapter, BoxMLElement> content;
};

#endif // __MathMLBoxMLAdapter_hh__
