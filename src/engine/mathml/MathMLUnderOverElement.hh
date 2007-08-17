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

#ifndef __MathMLUnderOverElement_hh__
#define __MathMLUnderOverElement_hh__

#include "MathMLEmbellishment.hh"
#include "MathMLContainerElement.hh"
#include "BinContainerTemplate.hh"

class GMV_MathView_EXPORT MathMLUnderOverElement
  : public MathMLContainerElement, public MathMLEmbellishment
{
protected:
  MathMLUnderOverElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLUnderOverElement();

public:
  static SmartPtr<MathMLUnderOverElement> create(const SmartPtr<class MathMLNamespaceContext>& view)
  { return new MathMLUnderOverElement(view); }

  void setBase(const SmartPtr<MathMLElement>& child) { base.setChild(this, child); }
  void setUnderScript(const SmartPtr<MathMLElement>& child) { underScript.setChild(this, child); }
  void setOverScript(const SmartPtr<MathMLElement>& child) { overScript.setChild(this, child); }
  SmartPtr<MathMLElement> getBase(void) const { return base.getChild(); }
  SmartPtr<MathMLElement> getUnderScript(void) const { return underScript.getChild(); }
  SmartPtr<MathMLElement> getOverScript(void) const { return overScript.getChild(); }

  virtual AreaRef format(class FormattingContext&);

  virtual void setDirtyAttribute(void);
  virtual void setFlagDown(Flags);
  virtual void resetFlagDown(Flags);

  virtual SmartPtr<class MathMLOperatorElement> getCoreOperator(void);

protected:
  bool   scriptize;

  bool   accentUnder;
  bool   accent;

  scaled baseShiftX;

  scaled underSpacing;
  scaled underShiftX;
  scaled underShiftY;

  scaled overSpacing;
  scaled overShiftX;
  scaled overShiftY;

  BinContainerTemplate<MathMLUnderOverElement,MathMLElement> base;
  BinContainerTemplate<MathMLUnderOverElement,MathMLElement> underScript;
  BinContainerTemplate<MathMLUnderOverElement,MathMLElement> overScript;
};

#endif // __MathMLUnderOverElement_hh__
