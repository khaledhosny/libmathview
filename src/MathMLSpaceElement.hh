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

#ifndef MathMLSpaceElement_hh
#define MathMLSpaceElement_hh

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLElement.hh"

class MathMLSpaceElement: public MathMLElement
{
protected:
  MathMLSpaceElement(void);
#if defined(HAVE_GMETADOM)
  MathMLSpaceElement(const DOM::Element&);
#endif
  virtual ~MathMLSpaceElement();

public:
  static SmartPtr<MathMLElement> create(void)
  { return SmartPtr<MathMLElement>(new MathMLSpaceElement()); }
#if defined(HAVE_GMETADOM)
  static SmartPtr<MathMLElement> create(const DOM::Element& el)
  { return SmartPtr<MathMLElement>(new MathMLSpaceElement(el)); }
#endif

  virtual void    Normalize(const SmartPtr<class MathMLDocument>&);
  virtual void    refine(class AbstractRefinementContext&);
  virtual void    Setup(class RenderingEnvironment&);
  virtual void    DoLayout(const class FormattingContext&);

  virtual bool    IsSpace(void) const;
  virtual bool    IsSpaceLike(void) const;
  BreakId         GetBreakability(void) const { return breakability; }
  virtual scaled  GetRightEdge(void) const;

private:
  bool    lineBreak;
  bool    autoLineBreak; // valid if lineBreaking == true
  BreakId breakability; // valid if auto == false
};

#endif // MathMLSpaceElement_hh
