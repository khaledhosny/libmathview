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

#ifndef __MathMLUnderOverElement_hh__
#define __MathMLUnderOverElement_hh__

#include "MathMLEmbellishment.hh"
#include "MathMLContainerElement.hh"
#include "BinContainerTemplate.hh"

class MathMLUnderOverElement
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

  virtual AreaRef format(class MathFormattingContext&);

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
