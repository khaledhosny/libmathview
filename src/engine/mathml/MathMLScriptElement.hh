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

#ifndef __MathMLScriptElement_hh__
#define __MathMLScriptElement_hh__

#include "MathMLEmbellishment.hh"
#include "MathMLContainerElement.hh"
#include "BinContainerTemplate.hh"

class GMV_MathView_EXPORT MathMLScriptElement
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
