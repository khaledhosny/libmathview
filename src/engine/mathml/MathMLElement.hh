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

#ifndef __MathMLElement_hh__
#define __MathMLElement_hh__

#include "Area.hh"
#include "Element.hh"
#include "token.hh"

// MathMLElement: base class for every MathML Element
class MathMLElement : public Element
{
protected:
  MathMLElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLElement();

public:
  virtual void construct(void);
  virtual void refine(class AbstractRefinementContext&);
  virtual AreaRef format(class MathFormattingContext&);

  virtual bool IsSpaceLike(void) const;

  virtual SmartPtr<class MathMLOperatorElement> getCoreOperator(void);
  SmartPtr<class MathMLOperatorElement> getCoreOperatorTop(void);

#if defined(HAVE_GMETADOM)
  SmartPtr<MathMLElement> getFormattingNode(const DOM::Element& el) const;
  bool IsSet(TokenId) const;
  TokenId IsA(void) const;
#endif // HAVE_GMETADOM

  virtual SmartPtr<class NamespaceContext> getNamespaceContext(void) const;
  SmartPtr<class MathMLNamespaceContext> getMathMLNamespaceContext(void) const;
  SmartPtr<class MathMLElementFactory> getFactory(void) const;

private:
  WeakPtr<class MathMLNamespaceContext> context;
};

#endif // __MathMLElement_hh__
