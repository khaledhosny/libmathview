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

#ifndef __MathMLLinearContainerElement_hh__
#define __MathMLLinearContainerElement_hh__

#include <vector>

#include "MathMLContainerElement.hh"
#include "LinearContainerTemplate.hh"

class MathMLLinearContainerElement : public MathMLContainerElement
{
protected:
  MathMLLinearContainerElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLLinearContainerElement();

public:
  unsigned getSize(void) const { return content.getSize(); }
  void setSize(unsigned size) { content.setSize(this, size); }

  SmartPtr<MathMLElement> getChild(unsigned i) const { return content.getChild(i); }
  void setChild(unsigned i, const SmartPtr<MathMLElement>& child) { content.setChild(this, i, child); }
  void appendChild(const SmartPtr<MathMLElement>& child) { content.appendChild(this, child); }
  void swapContent(std::vector<SmartPtr<MathMLElement> >& newContent) { content.swapContent(this, newContent); }
  // the content can be accessed directly, but only in a read-only
  // way, because other operation involves SetParent and other
  // memory-management issues
  const std::vector< SmartPtr<MathMLElement> >& getContent(void) const { return content.getContent(); }

  virtual void setFlagDown(Flags);
  virtual void resetFlagDown(Flags);

protected:
  LinearContainerTemplate<MathMLLinearContainerElement,MathMLElement> content;
};

#endif // __MathMLLinearContainerElement_hh__
