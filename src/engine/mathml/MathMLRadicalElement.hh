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

#ifndef __MathMLRadicalElement_hh__
#define __MathMLRadicalElement_hh__

#include "MathMLContainerElement.hh"

class MathMLRadicalElement : public MathMLContainerElement
{
protected:
  MathMLRadicalElement(const SmartPtr<class MathMLView>&);
  virtual ~MathMLRadicalElement();

public:
  static SmartPtr<MathMLRadicalElement> create(const SmartPtr<class MathMLView>& view)
  { return new MathMLRadicalElement(view); }

  virtual void construct(void);
  virtual void refine(class AbstractRefinementContext&);
  virtual AreaRef format(class MathFormattingContext&);
  virtual void setFlagDown(Flags);
  virtual void resetFlagDown(Flags);

  SmartPtr<class MathMLElement> getBase(void) const { return base; }
  SmartPtr<class MathMLElement> getIndex(void) const { return index; }
  void setBase(const SmartPtr<class MathMLElement>&);
  void setIndex(const SmartPtr<class MathMLElement>&);

private:
  SmartPtr<class MathMLElement> base;
  SmartPtr<class MathMLElement> index;
};

#endif // __MathMLRadicalElement_hh__
