// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#ifndef __BoxMLObjectElement_hh__
#define __BoxMLObjectElement_hh__

#include "BoxMLElement.hh"
#include "BinContainerTemplate.hh"

class BoxMLObjectElement : public BoxMLElement
{
protected:
  BoxMLObjectElement(const SmartPtr<class BoxMLNamespaceContext>&);
  virtual ~BoxMLObjectElement();

public:
  static SmartPtr<BoxMLObjectElement> create(const SmartPtr<class BoxMLNamespaceContext>&);

  virtual void construct(void);
  virtual AreaRef format(class BoxFormattingContext&);

  SmartPtr<Element> getChild(void) const { return content.getChild(); }
  void setChild(const SmartPtr<Element>& child) { content.setChild(this, child); }

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

private:
  SmartPtr<class NamespaceContext> contentContext;
  BinContainerTemplate<BoxMLObjectElement,Element> content;
};

#endif // __BoxMLObjectElement_hh__
