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

#ifndef MathMLBinContainerElement_hh
#define MathMLBinContainerElement_hh

#include "MathMLContainerElement.hh"

// base class for every non-empty MathML container element
class MathMLBinContainerElement : public MathMLContainerElement
{
protected:
  MathMLBinContainerElement(const SmartPtr<class MathMLView>& view);
  virtual ~MathMLBinContainerElement();

public:
  virtual void construct(void);
  virtual void refine(class AbstractRefinementContext&);
  virtual void Setup(RenderingEnvironment&);
  virtual void DoLayout(const class FormattingContext&);
  virtual AreaRef format(class MathFormattingContext&);
  virtual void SetPosition(const scaled&, const scaled&);
  virtual void Render(const DrawingArea&);
  virtual void ReleaseGCs(void);
  virtual SmartPtr<MathMLElement> Inside(const scaled&, const scaled&);

  virtual void SetDirty(const Rectangle* = 0);

  virtual scaled GetLeftEdge(void) const;
  virtual scaled GetRightEdge(void) const;

  virtual void Remove(const SmartPtr<MathMLElement>&);
  virtual void Replace(const SmartPtr<MathMLElement>&, const SmartPtr<MathMLElement>&);

  SmartPtr<MathMLElement> GetChild(void) const { return child; }
  void         SetChild(const SmartPtr<MathMLElement>&);

  virtual void SetFlagDown(Flags);
  virtual void ResetFlagDown(Flags);

protected:
  SmartPtr<MathMLElement> child;
};

#endif // MathMLContainerElement_hh
