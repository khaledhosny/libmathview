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

#include <config.h>

#include <cassert>

#include "MathMLRadicalElement.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLRadicalElement::MathMLRadicalElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLContainerElement(context)
{ }

MathMLRadicalElement::~MathMLRadicalElement()
{ }

AreaRef
MathMLRadicalElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      AreaRef baseArea = getBase()->format(ctxt);
      AreaRef indexArea;
      if (getIndex())
	{
	  ctxt.setDisplayStyle(false);
	  ctxt.addScriptLevel(2);
	  indexArea = getIndex()->format(ctxt);
	}
      AreaRef res = ctxt.getDevice()->radical(ctxt, baseArea, indexArea);

      setArea(ctxt.getDevice()->wrapper(ctxt, res));

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}

void
MathMLRadicalElement::setFlagDown(Flags f)
{
  MathMLContainerElement::setFlagDown(f);
  base.setFlagDown(f);
  index.setFlagDown(f);
}

void
MathMLRadicalElement::resetFlagDown(Flags f)
{
  MathMLContainerElement::resetFlagDown(f);
  base.resetFlagDown(f);
  index.resetFlagDown(f);
}

