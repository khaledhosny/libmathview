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

#ifndef __MathMLMultiScriptsElement_hh__
#define __MathMLMultiScriptsElement_hh__

#include "MathMLEmbellishment.hh"
#include "MathMLContainerElement.hh"
#include "BinContainerTemplate.hh"
#include "LinearContainerTemplate.hh"

class MathMLMultiScriptsElement
  : public MathMLContainerElement, public MathMLEmbellishment
{
protected:
  MathMLMultiScriptsElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLMultiScriptsElement();

public:
  static SmartPtr<MathMLMultiScriptsElement> create(const SmartPtr<class MathMLNamespaceContext>& view)
  { return new MathMLMultiScriptsElement(view); }

  unsigned getScriptsSize(void) const { return subScript.getSize(); }
  void setScriptsSize(unsigned size) { subScript.setSize(this, size); }
  unsigned getPreScriptsSize(void) const { return preSubScript.getSize(); }
  void setPreScriptsSize(unsigned size) { preSubScript.setSize(this, size); }
  void setBase(const SmartPtr<MathMLElement>& child) { base.setChild(this, child); }
  void setSubScript(unsigned i, const SmartPtr<MathMLElement>& child) { subScript.setChild(this, i, child); }
  void setSuperScript(unsigned i, const SmartPtr<MathMLElement>& child) { superScript.setChild(this, i, child); }
  void setPreSubScript(unsigned i, const SmartPtr<MathMLElement>& child) { preSubScript.setChild(this, i, child); }
  void setPreSuperScript(unsigned i, const SmartPtr<MathMLElement>& child) { preSuperScript.setChild(this, i, child); }
  SmartPtr<MathMLElement> getBase(void) const { return base.getChild(); }
  SmartPtr<MathMLElement> getSubScript(unsigned i) const { return subScript.getChild(i); }
  SmartPtr<MathMLElement> getSuperScript(unsigned i) const { return superScript.getChild(i); }
  SmartPtr<MathMLElement> getPreSubScript(unsigned i) const { return preSubScript.getChild(i); }
  SmartPtr<MathMLElement> getPreSuperScript(unsigned i) const { return preSuperScript.getChild(i); }

#if 0
  virtual void construct(void);
  virtual void refine(class AbstractRefinementContext&);
#endif
  virtual AreaRef format(class MathFormattingContext&);

  virtual void setFlagDown(Flags);
  virtual void resetFlagDown(Flags);
  virtual SmartPtr<class MathMLOperatorElement> getCoreOperator(void);

private:
  BinContainerTemplate<MathMLMultiScriptsElement,MathMLElement> base;
  LinearContainerTemplate<MathMLMultiScriptsElement,MathMLElement> subScript;
  LinearContainerTemplate<MathMLMultiScriptsElement,MathMLElement> superScript;
  LinearContainerTemplate<MathMLMultiScriptsElement,MathMLElement> preSubScript;
  LinearContainerTemplate<MathMLMultiScriptsElement,MathMLElement> preSuperScript;

  scaled subShiftX;
  scaled subShiftY;

  scaled superShiftX;
  scaled superShiftY;
};

#endif // __MathMLMultiScriptsElement_hh__
