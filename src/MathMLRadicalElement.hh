// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
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
  static Ptr<MathMLElement> create(void)
  { return Ptr<MathMLElement>(new MathMLRadicalElement()); }
#if defined(HAVE_GMETADOM)
  static Ptr<MathMLElement> create(const DOM::Element& el)
  { return Ptr<MathMLElement>(new MathMLRadicalElement(el)); }
#endif

  virtual void Normalize(const Ptr<class MathMLDocument>&);
  virtual void Setup(RenderingEnvironment&);
  virtual void DoLayout(const class FormattingContext&);
  virtual void SetPosition(scaled, scaled);
#if 0
  virtual void SetDirtyLayout(bool = false);
  virtual void SetDirty(const Rectangle* = 0);
  virtual void SetSelected(void);
  virtual void ResetSelected(void);
#endif
  virtual void SetFlagDown(Flags);
  virtual void ResetFlagDown(Flags);
  virtual void Render(const DrawingArea&);

  virtual void Replace(const Ptr<class MathMLElement>&, const Ptr<class MathMLElement>&);

  virtual scaled GetRightEdge(void) const;
  virtual scaled GetLeftEdge(void) const;
  virtual void ReleaseGCs(void);
  virtual Ptr<class MathMLElement> Inside(scaled, scaled);

  Ptr<class MathMLElement> GetRadicand(void) const { return radicand; }
  Ptr<class MathMLElement> GetIndex(void) const { return index; }
  void SetRadicand(const Ptr<class MathMLElement>&);
  void SetIndex(const Ptr<class MathMLElement>&);

protected:
  void Init(void);

private:
  Ptr<class MathMLElement> GetNucleus(void) const;
  Ptr<class MathMLCharNode> GetRadicalSign(void) const;
  void DoBaseLayoutAux(BoundingBox&, const BoundingBox&);

  scaled spacing;
  scaled lineThickness;

  Ptr<class MathMLCharNode> radical;
  Ptr<class MathMLElement> radicand;
  Ptr<class MathMLElement> index;

  RGBValue color;
};

#endif // MathMLRadicalElement_hh
