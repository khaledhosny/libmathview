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

#ifndef __MathMLLinearContainerElement_hh__
#define __MathMLLinearContainerElement_hh__

#include <vector>

#include "MathMLContainerElement.hh"
#include "LinearContainerTemplate.hh"

class MathMLLinearContainerElement : public MathMLContainerElement
{
protected:
  MathMLLinearContainerElement(const SmartPtr<class MathMLView>&);
  virtual ~MathMLLinearContainerElement();

public:
  unsigned getSize(void) const { return content.getSize(); }
  void setSize(unsigned size) { content.setSize(this, size); }

  SmartPtr<MathMLElement> getChild(unsigned i) const { return content.getChild(i); }
  void setChild(unsigned i, const SmartPtr<MathMLElement>& child) { content.setChild(this, i, child); }
  void appendChild(const SmartPtr<MathMLElement>& child) { content.appendChild(this, child); }

  virtual void construct(void);
  virtual void refine(class AbstractRefinementContext&);

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
