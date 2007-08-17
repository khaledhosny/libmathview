// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
