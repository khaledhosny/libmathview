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

#include "MathMLStringLitElement.hh"
#include "MathMLStringNode.hh"
#include "ValueConversion.hh"
#include "MathMLAttributeSignatures.hh"

MathMLStringLitElement::MathMLStringLitElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLTokenElement(context)
{
  setupDone = false;
}

MathMLStringLitElement::~MathMLStringLitElement()
{ }

#if 0
void
MathMLStringLitElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, MathML, StringLit, lquote);
      REFINE_ATTRIBUTE(context, MathML, StringLit, rquote);
      MathMLTokenElement::refine(context);
    }
}
#endif

#if 0
void
MathMLStringLitElement::Setup(RenderingEnvironment& env)
{
  if (dirtyAttribute())
    {
      if (setupDone)
	{
	  assert(GetSize() >= 2);
	  RemoveChild(GetSize() - 1);
	  RemoveChild(0);
	}

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(StringLit, lquote))
	{
	  String s = ToString(value);
	  if (!s.empty()) InsertChild(0, MathMLStringNode::create(toDOMString(s)));
	}

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(StringLit, rquote))
	{
	  String s = ToString(value);
	  if (!s.empty()) InsertChild(GetSize(), MathMLStringNode::create(toDOMString(s)));
	}

      MathMLTokenElement::Setup(env);

      setupDone = true;

      resetDirtyAttribute();
    }
}
#endif
