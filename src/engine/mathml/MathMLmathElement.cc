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

#include "MathMLmathElement.hh"
#include "ValueConversion.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "MathMLAttributeSignatures.hh"

MathMLmathElement::MathMLmathElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLNormalizingContainerElement(context)
{ }

MathMLmathElement::~MathMLmathElement()
{ }

AreaRef
MathMLmathElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, math, display);
      assert(value);
      ctxt.setDisplayStyle(ToTokenId(value) == T_BLOCK);

      AreaRef res = getChild() ? getChild()->format(ctxt) : 0;
      if (res) res = ctxt.MGD()->wrapper(ctxt, res);
      setArea(res);

      ctxt.pop();

      resetDirtyLayout();
    }

  return getArea();
}
