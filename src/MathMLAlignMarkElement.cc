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

#include "AttributeParser.hh"
#include "MathMLAlignMarkElement.hh"
#include "FormattingContext.hh"
#include "ValueConversion.hh"

MathMLAlignMarkElement::MathMLAlignMarkElement()
{
  edge = MARK_ALIGN_NOTVALID;
}
 
#if defined(HAVE_GMETADOM)
MathMLAlignMarkElement::MathMLAlignMarkElement(const DOM::Element& node)
  : MathMLElement(node)
{
  edge = MARK_ALIGN_NOTVALID;
}
#endif

MathMLAlignMarkElement::~MathMLAlignMarkElement()
{
}

void
MathMLAlignMarkElement::Normalize(const SmartPtr<class MathMLDocument>&)
{
  if (DirtyStructure()) ResetDirtyStructure();
}

void
MathMLAlignMarkElement::refine(AbstractRefinementContext& context)
{
  if (DirtyAttribute())
    {
      REFINE_ATTRIBUTE(context, AlignMark, edge);
      MathMLElement::refine(context);
    }
}

void
MathMLAlignMarkElement::Setup(RenderingEnvironment&)
{
  if (DirtyAttribute())
    {
      edge = ToMarkAlignId(GET_ATTRIBUTE_VALUE(AlignMark, edge));
      assert(edge != MARK_ALIGN_NOTVALID);
      box.unset();
      ResetDirtyAttribute();
    }
}

void
MathMLAlignMarkElement::DoLayout(const FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt)) ResetDirtyLayout(ctxt);
}

bool
MathMLAlignMarkElement::IsSpaceLike() const
{
  return true;
}

void
MathMLAlignMarkElement::SetDirty(const Rectangle*)
{
  // do nothing, this element is space-like and always 0-width, useless for it to be dirty
}
