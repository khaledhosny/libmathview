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

MathMLSpaceElement::MathMLSpaceElement(mDOMNodeRef node) :
  MathMLElement(node, TAG_MSPACE)
{
  breakability = BREAK_AUTO;
}

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
MathMLSpaceElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  background = env->GetBackgroundColor();

  const Value* value = NULL;

  value = GetAttributeValue(ATTR_WIDTH);
  assert(value != NULL && value->IsNumberUnit());
  width = env->ToScaledPoints(value->ToNumberUnit());
  delete value;

  value = GetAttributeValue(ATTR_HEIGHT);
  assert(value != NULL && value->IsNumberUnit());
  height = env->ToScaledPoints(value->ToNumberUnit());
  delete value;

  value = GetAttributeValue(ATTR_DEPTH);
  assert(value != NULL && value->IsNumberUnit());
  depth = env->ToScaledPoints(value->ToNumberUnit());
  delete value;

  if (!IsSet(ATTR_WIDTH) && !IsSet(ATTR_HEIGHT) && !IsSet(ATTR_DEPTH)) {
    value = GetAttributeValue(ATTR_LINEBREAK);
    assert(value != NULL && value->IsKeyword());
    switch (value->ToKeyword()) {
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
}

void
MathMLSpaceElement::DoBoxedLayout(LayoutId id, BreakId bid, scaled maxWidth)
{
  box.Set(width, height, depth);
  ConfirmLayout(id);
}

bool
MathMLSpaceElement::IsSpaceLike() const
{
  return true;
}

BreakId
MathMLSpaceElement::GetBreakability() const
{
  return breakability;
}

scaled
MathMLSpaceElement::GetRightEdge() const
{
  return GetX() + (lineBreak ? width : 0);
}
