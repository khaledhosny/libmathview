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

#include "MathEngine.hh"
#include "StringUnicode.hh"
#include "MathMLmathElement.hh"
#include "RenderingEnvironment.hh"

MathMLmathElement::MathMLmathElement(mDOMNodeRef node) :
  MathMLNormalizingContainerElement(node, TAG_MATH)
{
}

MathMLmathElement::~MathMLmathElement()
{
}

const AttributeSignature*
MathMLmathElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_MODE,     modeParser,    new StringC("inline"), NULL },
    { ATTR_DISPLAY,  displayParser, new StringC("inline"), NULL },
    { ATTR_NOTVALID, NULL,          NULL,                  NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLNormalizingContainerElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLmathElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  env->Push();

  const Value* value = NULL;

  env->SetFontMode(FONT_MODE_MATH);

  if (!IsSet(ATTR_MODE) || IsSet(ATTR_DISPLAY)) {
    value = GetAttributeValue(ATTR_DISPLAY, env, true);
    assert(value != NULL);
    if (value->IsKeyword(KW_BLOCK)) env->SetDisplayStyle(true);
    else env->SetDisplayStyle(false);
  } else {
    MathEngine::logger(LOG_WARNING, "attribute `mode' is deprecated in MathML 2");
    value = GetAttributeValue(ATTR_MODE, env, true);
    assert(value != NULL);
    if (value->IsKeyword(KW_DISPLAY)) env->SetDisplayStyle(true);
    else env->SetDisplayStyle(false);
  }

  delete value;

  if (IsSet(ATTR_MODE) && IsSet(ATTR_DISPLAY))
    MathEngine::logger(LOG_WARNING, "both `mode' and `display' attributes set in `math' element");

  MathMLNormalizingContainerElement::Setup(env);

  env->Drop();
}

bool
MathMLmathElement::IsBreakable() const
{
  return true;
}

