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

#ifndef MathMLLinearContainerElement_hh
#define MathMLLinearContainerElement_hh

#include <vector>

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLContainerElement.hh"

class MathMLLinearContainerElement: public MathMLContainerElement
{
protected:
  MathMLLinearContainerElement(void);
#if defined(HAVE_GMETADOM)
  MathMLLinearContainerElement(const DOM::Element&);
#endif
  virtual ~MathMLLinearContainerElement();

public:
  unsigned     GetSize(void) const { return content.size(); }
  void         SetSize(unsigned);
  Ptr<MathMLElement> GetChild(unsigned) const;
  void         SetChild(unsigned, const Ptr<MathMLElement>&);
  virtual void Append(const Ptr<MathMLElement>&);
  virtual void Replace(const Ptr<MathMLElement>&, const Ptr<MathMLElement>&);
  void         SwapChildren(std::vector< Ptr<MathMLElement> >&);

  virtual void Normalize(const Ptr<class MathMLDocument>&);
  virtual void Setup(class RenderingEnvironment&);
  virtual void DoLayout(const class FormattingContext&);
  virtual void Render(const DrawingArea&);
  virtual void ReleaseGCs(void);
  virtual Ptr<MathMLElement> Inside(scaled, scaled);

  virtual scaled GetLeftEdge(void) const;
  virtual scaled GetRightEdge(void) const;

  // the content can be accessed directly, but only in a read-only
  // way, because other operation involves SetParent and other
  // memory-management issues
  const std::vector< Ptr<MathMLElement> >& GetContent(void) const { return content; }

  virtual void SetFlagDown(Flags);
  virtual void ResetFlagDown(Flags);

  std::vector< Ptr<MathMLElement> > content;
};

#endif // MathMLLinearContainerElement_hh
