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
#include <assert.h>
#include <stddef.h>

#include "StringUnicode.hh"
#include "MathMLSpaceElement.hh"
#include "RenderingEnvironment.hh"
#include "AttributeParser.hh"
#include "FormattingContext.hh"

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
MathMLSpaceElement::Normalize(const Ptr<class MathMLDocument>&)
{
  if (DirtyStructure()) ResetDirtyStructure();
}

void
MathMLSpaceElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute())
    {
      background = env.GetBackgroundColor();

      const Value* value = NULL;

      value = GetAttributeValue(ATTR_WIDTH);
      assert(value != NULL);
      scaled width;
      if (value->IsKeyword())
	{
	  const Value* resValue = Resolve(value, env);
	  assert(resValue->IsNumberUnit());
	  width = env.ToScaledPoints(resValue->ToNumberUnit());
	  delete resValue;
	}
      else
	width = env.ToScaledPoints(value->ToNumberUnit());
      delete value;

      value = GetAttributeValue(ATTR_HEIGHT);
      assert(value != NULL && value->IsNumberUnit());
      scaled height = env.ToScaledPoints(value->ToNumberUnit());
      delete value;

      value = GetAttributeValue(ATTR_DEPTH);
      assert(value != NULL && value->IsNumberUnit());
      scaled depth = env.ToScaledPoints(value->ToNumberUnit());
      delete value;

      box.Set(width, height, depth);

      if (!IsSet(ATTR_WIDTH) && !IsSet(ATTR_HEIGHT) && !IsSet(ATTR_DEPTH))
	{
	  value = GetAttributeValue(ATTR_LINEBREAK);
	  assert(value != NULL && value->IsKeyword());
	  switch (value->ToKeyword())
	    {
	    case KW_AUTO:
	      breakability = BREAK_AUTO;
	      break;
	    case KW_NEWLINE:
	      breakability = BREAK_YES;
	      break;
	    case KW_INDENTINGNEWLINE:
	      breakability = BREAK_INDENT;
	      break;
	    case KW_NOBREAK:
	      breakability = BREAK_NO;
	      break;
	    case KW_BADBREAK:
	      breakability = BREAK_BAD;
	      break;
	    case KW_GOODBREAK:
	      breakability = BREAK_GOOD;
	      break;
	    default:
	      assert(IMPOSSIBLE);
	      break;
	    }
	  delete value;
	}

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
