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

#ifndef MathMLContainerElement_hh
#define MathMLContainerElement_hh

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLElement.hh"

// base class for every non-empty MathML container element
class MathMLContainerElement: public MathMLElement
{
protected:
  MathMLContainerElement(void);
#if defined(HAVE_GMETADOM)
  MathMLContainerElement(const DOM::Element&);
#endif
  virtual ~MathMLContainerElement();

public:
  virtual void Render(const DrawingArea&) = 0;
  virtual void ReleaseGCs(void) = 0;
  virtual Ptr<class MathMLElement> Inside(scaled, scaled) = 0;

  virtual scaled GetLeftEdge(void) const = 0;
  virtual scaled GetRightEdge(void) const = 0;

  virtual void Replace(const Ptr<class MathMLElement>&, const Ptr<class MathMLElement>&) = 0;
};

#endif // MathMLContainerElement_hh
