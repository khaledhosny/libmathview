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

#include "MathMLNormalizingContainerElement.hh"

class MathMLRadicalElement : public MathMLNormalizingContainerElement {
public:
  MathMLRadicalElement(mDOMNodeRef, TagId);
  virtual void Normalize(void);
  virtual void Setup(RenderingEnvironment*);
  virtual void DoBoxedLayout(LayoutId, BreakId, scaled);
  virtual void SetPosition(scaled, scaled);
  virtual void Render(const DrawingArea&);
  virtual ~MathMLRadicalElement();

  virtual bool IsExpanding(void) const;
  virtual scaled GetLeftEdge(void) const;

private:
  class MathMLElement* GetNucleus(void) const;
  class MathMLCharNode* GetRadicalSign(void) const;
  void DoBaseLayoutAux(BoundingBox&, const BoundingBox&);

  scaled spacing;
  scaled lineThickness;

  MathMLCharNode* radical;

  RGBValue color;
};

#endif // MathMLRadicalElement_hh
