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

#include <config.h>

#include <cassert>
#include <stddef.h>

#include "StringUnicode.hh"
#include "MathMLSpaceElement.hh"
#include "RenderingEnvironment.hh"
#include "AttributeParser.hh"
#include "FormattingContext.hh"
#include "ValueConversion.hh"

MathMLSpaceElement::MathMLSpaceElement()
{
  breakability = BREAK_AUTO;
}

#if defined(HAVE_GMETADOM)
MathMLSpaceElement::MathMLSpaceElement(const DOM::Element& node)
  : MathMLElement(node)
{
  breakability = BREAK_AUTO;
}
#endif

MathMLSpaceElement::~MathMLSpaceElement()
{
}

const AttributeSignature*
MathMLSpaceElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_WIDTH,     spaceParser,      new StringC("0em"),  NULL },
    { ATTR_HEIGHT,    numberUnitParser, new StringC("0ex"),  NULL },
    { ATTR_DEPTH,     numberUnitParser, new StringC("0ex"),  NULL },
    { ATTR_LINEBREAK, lineBreakParser,  new StringC("auto"), NULL },
    { ATTR_NOTVALID,  NULL,             NULL,                NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLSpaceElement::Normalize(const SmartPtr<class MathMLDocument>&)
{
  if (DirtyStructure()) ResetDirtyStructure();
}

void
MathMLSpaceElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute())
    {
      background = env.GetBackgroundColor();

      scaled width;
      if (SmartPtr<Value> value = GetAttributeValue(ATTR_WIDTH))
	if (IsKeyword(value))
	  width = env.ToScaledPoints(ToNumberUnit(Resolve(value, env)));
	else
	  width = env.ToScaledPoints(ToNumberUnit(value));
      else
	assert(IMPOSSIBLE);

      scaled height;
      if (SmartPtr<Value> value = GetAttributeValue(ATTR_HEIGHT))
	height = env.ToScaledPoints(ToNumberUnit(value));

      scaled depth;
      if (SmartPtr<Value> value = GetAttributeValue(ATTR_DEPTH))
	depth = env.ToScaledPoints(ToNumberUnit(value));

      box.set(width, height, depth);

      if (!IsSet(ATTR_WIDTH) && !IsSet(ATTR_HEIGHT) && !IsSet(ATTR_DEPTH))
	breakability = ToBreakId(GetAttributeValue(ATTR_LINEBREAK));

      ResetDirtyAttribute();
    }
}

void
MathMLSpaceElement::DoLayout(const FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt)) ResetDirtyLayout(ctxt);
}

bool
MathMLSpaceElement::IsSpace() const
{
  return true;
}

bool
MathMLSpaceElement::IsSpaceLike() const
{
  return true;
}

scaled
MathMLSpaceElement::GetRightEdge() const
{
  return GetX() + (lineBreak ? box.width : 0);
}
