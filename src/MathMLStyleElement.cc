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

#include "stringAux.hh"
#include "StringUnicode.hh"
#include "ValueConversion.hh"
#include "MathMLAttribute.hh"
#include "MathMLStyleElement.hh"
#include "MathMLAttributeList.hh"
#include "RenderingEnvironment.hh"

MathMLStyleElement::MathMLStyleElement(mDOMNodeRef node) :
  MathMLNormalizingContainerElement(node, TAG_MSTYLE)
{
}

MathMLStyleElement::~MathMLStyleElement()
{
}

const AttributeSignature*
MathMLStyleElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_SCRIPTLEVEL,            scriptLevelParser, NULL },
    { ATTR_DISPLAYSTYLE,           booleanParser,     NULL },
    { ATTR_SCRIPTSIZEMULTIPLIER,   numberParser,      NULL },
    { ATTR_SCRIPTMINSIZE,          numberUnitParser,  NULL },
    { ATTR_COLOR,                  colorParser,       NULL },
    { ATTR_BACKGROUND,             backgroundParser,  NULL },
    { ATTR_VERYVERYTHINMATHSPACE,  numberUnitParser,  NULL },
    { ATTR_VERYTHINMATHSPACE,      numberUnitParser,  NULL },
    { ATTR_THINMATHSPACE,          numberUnitParser,  NULL },
    { ATTR_MEDIUMMATHSPACE,        numberUnitParser,  NULL },
    { ATTR_THICKMATHSPACE,         numberUnitParser,  NULL },
    { ATTR_VERYTHICKMATHSPACE,     numberUnitParser,  NULL },
    { ATTR_VERYVERYTHICKMATHSPACE, numberUnitParser,  NULL },
    // inherited attributes (see below)
    { ATTR_FONTSIZE,   numberUnitParser, NULL },
    { ATTR_FONTWEIGHT, fontWeightParser, NULL },
    { ATTR_FONTSTYLE,  fontStyleParser,  new StringC("normal") },
    { ATTR_FONTFAMILY, stringParser,     NULL },
    //
    { ATTR_NOTVALID,               NULL,              NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLStyleElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  MathMLAttributeList attributes;

  for (mDOMAttrRef attribute = mdom_node_get_first_attribute(GetDOMNode());
       attribute != NULL;
       attribute = mdom_attr_get_next_sibling(attribute)) {
    AttributeId id = AttributeIdOfName(C_CONST_STRING(mdom_attr_get_name(attribute)));
    if (id != ATTR_NOTVALID) {
      mDOMStringRef value = mdom_attr_get_value(attribute);
      String* sValue = allocString(value);
      attributes.Append(new MathMLAttribute(id, sValue));
      mdom_string_free(value);
    }
  }
  
  env->Push(&attributes);

  const Value* value = NULL;

  value = GetAttributeValue(ATTR_DISPLAYSTYLE, NULL, false);
  if (value != NULL) env->SetDisplayStyle(value->ToBoolean());

  value = GetAttributeValue(ATTR_SCRIPTSIZEMULTIPLIER, NULL, false);
  if (value != NULL) env->SetScriptSizeMultiplier(value->ToNumber());

  value = GetAttributeValue(ATTR_SCRIPTMINSIZE, NULL, false);
  if (value != NULL) env->SetScriptMinSize(value->ToNumberUnit());

  value = GetAttributeValue(ATTR_SCRIPTLEVEL, NULL, false);
  if (value != NULL) {
    const Value* p = value->Get(0);
    assert(p != NULL);

    if (p->IsEmpty()) {
      p = value->Get(1);
      assert(p != NULL);
      
      int scriptLevel = p->ToInteger();
      if (scriptLevel < 0) scriptLevel = 0;
      env->SetScriptLevel(scriptLevel);
    } else {
      int sign = 1;
      if (p->IsKeyword(KW_PLUS)) sign = 1;
      else sign = -1;
      p = value->Get(1);
      assert(p != NULL);
      
      int scriptLevel = p->ToInteger();
      if (scriptLevel < 0) scriptLevel = 0;
      env->AddScriptLevel(sign * scriptLevel);
    }
  }
  
  value = GetAttributeValue(ATTR_COLOR, NULL, false);
  if (value != NULL) env->SetColor(ToRGB(value));

  RGBValue oldBackground = env->GetBackgroundColor();
  value = GetAttributeValue(ATTR_BACKGROUND, NULL, false);
  if (value != NULL) {
    if (!value->IsKeyword(KW_TRANSPARENT)) env->SetBackgroundColor(ToRGB(value));
  }
  background = env->GetBackgroundColor();
  differentBackground = background != oldBackground;

  value = GetAttributeValue(ATTR_VERYVERYTHINMATHSPACE, NULL, false);
  if (value != NULL) env->SetMathSpace(MATH_SPACE_VERYVERYTHIN, value->ToNumberUnit());

  value = GetAttributeValue(ATTR_VERYTHINMATHSPACE, NULL, false);
  if (value != NULL) env->SetMathSpace(MATH_SPACE_VERYTHIN, value->ToNumberUnit());

  value = GetAttributeValue(ATTR_THINMATHSPACE, NULL, false);
  if (value != NULL) env->SetMathSpace(MATH_SPACE_THIN, value->ToNumberUnit());

  value = GetAttributeValue(ATTR_MEDIUMMATHSPACE, NULL, false);
  if (value != NULL) env->SetMathSpace(MATH_SPACE_MEDIUM, value->ToNumberUnit());

  value = GetAttributeValue(ATTR_THICKMATHSPACE, NULL, false);
  if (value != NULL) env->SetMathSpace(MATH_SPACE_THICK, value->ToNumberUnit());

  value = GetAttributeValue(ATTR_VERYTHINMATHSPACE, NULL, false);
  if (value != NULL) env->SetMathSpace(MATH_SPACE_VERYTHICK, value->ToNumberUnit());

  value = GetAttributeValue(ATTR_VERYVERYTHICKMATHSPACE, NULL, false);
  if (value != NULL) env->SetMathSpace(MATH_SPACE_VERYVERYTHICK, value->ToNumberUnit());

  // the following attributes, thought not directly supported by <mstyle>
  // must be parsed here since they are always inherited by other elements

  value = GetAttributeValue(ATTR_FONTSIZE, NULL, false);
  if (value != NULL) env->SetFontSize(value->ToNumberUnit());

  value = GetAttributeValue(ATTR_FONTFAMILY, NULL, false);
  if (value != NULL) env->SetFontFamily(value->ToString());

  value = GetAttributeValue(ATTR_FONTWEIGHT, NULL, false);
  if (value != NULL) env->SetFontWeight(ToFontWeightId(value));

  value = GetAttributeValue(ATTR_FONTSTYLE, NULL, false);
  if (value != NULL) env->SetFontStyle(ToFontStyleId(value));

  MathMLNormalizingContainerElement::Setup(env);

  env->Drop();

  while (attributes.GetSize() > 0) {
    MathMLAttribute* attr = attributes.RemoveFirst();
    delete attr;
  }
}

void
MathMLStyleElement::Render(const DrawingArea& area)
{
  if (!HasDirtyChildren()) return;
 
  if (IsDirty()) {
    if (differentBackground) {
      if (bGC[0] == NULL) {
	GraphicsContextValues values;
	values.foreground = values.background = background;
	bGC[0] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_BACKGROUND);
      }

      assert(IsShaped());
      area.Clear(bGC[0], GetShape());
    }
  }

  assert(content.GetSize() == 1);
  MathMLElement* elem = content.GetFirst();
  assert(elem != NULL);

  elem->Render(area);

  ResetDirty();
}

bool
MathMLStyleElement::IsSpaceLike() const
{
  assert(content.GetSize() == 1);

  MathMLElement* elem = content.GetFirst();
  assert(elem != NULL);

  return elem->IsSpaceLike();
}

bool
MathMLStyleElement::IsBreakable() const
{
  return true;
}
