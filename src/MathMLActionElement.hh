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

#if defined(HAVE_MINIDOM)
#include <minidom.h>
#elif defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLContainerElement.hh"

class MathMLActionElement : public MathMLContainerElement {
public:
#if defined(HAVE_MINIDOM)
  MathMLActionElement(mDOMNodeRef);
#elif defined(HAVE_GMETADOM)
  MathMLActionElement(GMetaDOM::Element&);
#endif
  virtual const AttributeSignature* GetAttributeSignature(AttributeId) const;
  virtual void Setup(class RenderingEnvironment*);
  virtual void DoBoxedLayout(LayoutId, BreakId, scaled);
  virtual void DoLayout(LayoutId, class Layout&);
  virtual void DoStretchyLayout(void);
  virtual void SetPosition(scaled, scaled);
  virtual void Freeze(void);
  virtual void Render(const DrawingArea&);
  virtual ~MathMLActionElement();

  virtual void SetDirtyLayout(bool = false);
  virtual void SetDirty(const Rectangle* = NULL);  
  virtual void SetSelected(void);
  virtual void ResetSelected(void);
  virtual void ResetLast(void);

  virtual bool IsLast(void) const;
  virtual bool IsBreakable(void) const;
  virtual bool IsExpanding(void) const;
  virtual BreakId GetBreakability(void) const;
  virtual scaled GetLeftEdge(void) const;
  virtual scaled GetRightEdge(void) const;

  virtual MathMLElement* Inside(scaled, scaled);
  MathMLElement* GetSelectedElement(void) const;

  unsigned GetSelectedIndex(void) const;
  void     SetSelectedIndex(unsigned);

private:
  unsigned selection;
};

#define TO_ACTION(obj) (dynamic_cast<MathMLActionElement*>(obj))

#endif // MathMLActionElement_hh
