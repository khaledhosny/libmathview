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

#ifndef MathMLUnderOverElement_hh
#define MathMLUnderOverElement_hh

#include "MathMLEmbellishment.hh"
#include "MathMLContainerElement.hh"
#include "MathMLScriptCommonElement.hh"

class MathMLUnderOverElement
  : public MathMLContainerElement, private MathMLScriptCommonElement, public MathMLEmbellishment
{
protected:
  MathMLUnderOverElement(const SmartPtr<class MathMLView>&);
  virtual ~MathMLUnderOverElement();

public:
  static SmartPtr<MathMLUnderOverElement> create(const SmartPtr<class MathMLView>& view)
  { return new MathMLUnderOverElement(view); }

  void SetBase(const SmartPtr<MathMLElement>&);
  void SetUnderScript(const SmartPtr<MathMLElement>&);
  void SetOverScript(const SmartPtr<MathMLElement>&);
  SmartPtr<MathMLElement> GetBase(void) const { return base; }
  SmartPtr<MathMLElement> GetUnderScript(void) const { return underScript; }
  SmartPtr<MathMLElement> GetOverScript(void) const { return overScript; }
#if 0
  virtual void Replace(const SmartPtr<MathMLElement>&, const SmartPtr<MathMLElement>&);
#endif

  virtual void construct(void);
  virtual void refine(class AbstractRefinementContext&);
  virtual AreaRef format(class MathFormattingContext&);
#if 0
  virtual void Setup(RenderingEnvironment&);
  virtual void DoLayout(const class FormattingContext&);
  virtual void SetPosition(const scaled&, const scaled&);
  virtual void Render(const class DrawingArea&);
  virtual void ReleaseGCs(void);
#endif

  virtual void setDirtyAttribute(void);
  virtual void setFlagDown(Flags);
  virtual void resetFlagDown(Flags);
#if 0
  virtual scaled GetLeftEdge(void) const;
  virtual scaled GetRightEdge(void) const;
  virtual SmartPtr<MathMLElement> Inside(const scaled&, const scaled&);
#endif
  virtual SmartPtr<class MathMLOperatorElement> GetCoreOperator(void);

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

  SmartPtr<MathMLElement> underScript;
  SmartPtr<MathMLElement> overScript;
};

#endif // MathMLUnderOverElement_hh
