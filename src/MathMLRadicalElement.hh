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

#ifndef MathMLRadicalElement_hh
#define MathMLRadicalElement_hh

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLContainerElement.hh"

class MathMLRadicalElement : public MathMLContainerElement
{
protected:
  MathMLRadicalElement(void);
#if defined(HAVE_GMETADOM)
  MathMLRadicalElement(const DOM::Element&);
#endif
  virtual ~MathMLRadicalElement();

public:
  static SmartPtr<MathMLElement> create(void)
  { return SmartPtr<MathMLElement>(new MathMLRadicalElement()); }
#if defined(HAVE_GMETADOM)
  static SmartPtr<MathMLElement> create(const DOM::Element& el)
  { return SmartPtr<MathMLElement>(new MathMLRadicalElement(el)); }
#endif

  virtual void Normalize(const SmartPtr<class MathMLDocument>&);
  virtual void Setup(RenderingEnvironment&);
  virtual void DoLayout(const class FormattingContext&);
  virtual void SetPosition(const scaled&, const scaled&);
  virtual void SetFlagDown(Flags);
  virtual void ResetFlagDown(Flags);
  virtual void Render(const DrawingArea&);

  virtual void Replace(const SmartPtr<class MathMLElement>&, const SmartPtr<class MathMLElement>&);

  virtual scaled GetRightEdge(void) const;
  virtual scaled GetLeftEdge(void) const;
  virtual void ReleaseGCs(void);
  virtual SmartPtr<class MathMLElement> Inside(const scaled&, const scaled&);

  SmartPtr<class MathMLElement> GetRadicand(void) const { return radicand; }
  SmartPtr<class MathMLElement> GetIndex(void) const { return index; }
  void SetRadicand(const SmartPtr<class MathMLElement>&);
  void SetIndex(const SmartPtr<class MathMLElement>&);

protected:
  void Init(void);

private:
  SmartPtr<class MathMLElement> GetNucleus(void) const;
  SmartPtr<class MathMLCharNode> GetRadicalSign(void) const;
  void DoBaseLayoutAux(BoundingBox&, const BoundingBox&);

  scaled spacing;
  scaled lineThickness;

  //SmartPtr<class MathMLCharNode> radical;
  SmartPtr<class MathMLElement> radicand;
  SmartPtr<class MathMLElement> index;

  RGBValue color;
};

#endif // MathMLRadicalElement_hh
