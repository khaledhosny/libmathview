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

#ifndef __MathMLScriptElement_hh__
#define __MathMLScriptElement_hh__

#include "MathMLEmbellishment.hh"
#include "MathMLContainerElement.hh"
#include "BinContainerTemplate.hh"

class MathMLScriptElement
  : public MathMLContainerElement, public MathMLEmbellishment
{
protected:
  MathMLScriptElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLScriptElement();

public:
  static SmartPtr<MathMLScriptElement> create(const SmartPtr<class MathMLNamespaceContext>& view)
  { return new MathMLScriptElement(view); }

  void setBase(const SmartPtr<MathMLElement>& child) { base.setChild(this, child); }
  void setSubScript(const SmartPtr<MathMLElement>& child) { subScript.setChild(this, child); }
  void setSuperScript(const SmartPtr<MathMLElement>& child) { superScript.setChild(this, child); }
  SmartPtr<MathMLElement> getBase(void) const { return base.getChild(); }
  SmartPtr<MathMLElement> getSubScript(void) const { return subScript.getChild(); }
  SmartPtr<MathMLElement> getSuperScript(void) const { return superScript.getChild(); }

  virtual AreaRef format(class FormattingContext&);

  virtual void setFlagDown(Flags);
  virtual void resetFlagDown(Flags);
  virtual SmartPtr<class MathMLOperatorElement> getCoreOperator(void);

private:
  BinContainerTemplate<MathMLScriptElement,MathMLElement> base;
  BinContainerTemplate<MathMLScriptElement,MathMLElement> subScript;
  BinContainerTemplate<MathMLScriptElement,MathMLElement> superScript;
};

#endif // __MathMLScriptElement_hh__
