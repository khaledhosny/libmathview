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

#ifndef MathMLActionElement_hh
#define MathMLActionElement_hh

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLEmbellishment.hh"
#include "MathMLLinearContainerElement.hh"

class MathMLActionElement
  : public MathMLLinearContainerElement, public MathMLEmbellishment
{
protected:
  MathMLActionElement(void);
#if defined(HAVE_GMETADOM)
  MathMLActionElement(const DOM::Element&);
#endif
  virtual ~MathMLActionElement();

public:
  static Ptr<MathMLElement> create(void)
  { return Ptr<MathMLElement>(new MathMLActionElement()); }
#if defined(HAVE_GMETADOM)
  static Ptr<MathMLElement> create(const DOM::Element& el)
  { return Ptr<MathMLElement>(new MathMLActionElement(el)); }
#endif

  virtual const AttributeSignature* GetAttributeSignature(AttributeId) const;
  virtual void Setup(class RenderingEnvironment&);
  virtual void DoLayout(const class FormattingContext&);
  virtual void SetPosition(scaled, scaled);
  virtual void Render(const DrawingArea&);

#if 0
  virtual void SetDirtyLayout(bool = false);
  virtual void SetDirty(const Rectangle* = NULL);  
  virtual void SetSelected(void);
  virtual void ResetSelected(void);
#endif

  //virtual void SetFlagDown(Flags);
  //virtual void ResetFlagDown(Flags);

  virtual scaled GetLeftEdge(void) const;
  virtual scaled GetRightEdge(void) const;

  virtual Ptr<MathMLElement> Inside(scaled, scaled);
  Ptr<MathMLElement> GetSelectedElement(void) const;

  unsigned GetSelectedIndex(void) const;
  void     SetSelectedIndex(unsigned);

  virtual Ptr<MathMLOperatorElement> GetCoreOperator(void);

private:
  unsigned selection;
};

#endif // MathMLActionElement_hh
