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

#include "MathMLEncloseElement.hh"
#include "MathMLRadicalElement.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "ValueConversion.hh"
#include "MathMLAttributeSignatures.hh"

MathMLEncloseElement::MathMLEncloseElement(const SmartPtr<MathMLNamespaceContext>& context)
  : MathMLNormalizingContainerElement(context)
{ }

MathMLEncloseElement::~MathMLEncloseElement()
{ }

void
MathMLEncloseElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, MathML, Enclose, notation);
      MathMLNormalizingContainerElement::refine(context);
    }
}

AreaRef
MathMLEncloseElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);
      String notation = ToString(GET_ATTRIBUTE_VALUE(MathML, Enclose, notation));
      AreaRef res;
      if (getChild())
	res = ctxt.getDevice()->enclose(ctxt, getChild()->format(ctxt), notation);
      else
	res = 0;
      setArea(res ? ctxt.getDevice()->wrapper(ctxt, res) : 0);
      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
