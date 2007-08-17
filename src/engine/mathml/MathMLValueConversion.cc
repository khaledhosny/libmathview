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

#include "MathMLValueConversion.hh"
#include "FormattingContext.hh"

bool
isTokenId(const SmartPtr<Value>& value, TokenId id)
{ return IsTokenId(value) && ToTokenId(value) == id; }

MathVariant
toMathVariant(TokenId id)
{
  switch (id)
    {
    case T_NORMAL: return NORMAL_VARIANT;
    case T_BOLD: return BOLD_VARIANT;
    case T_ITALIC: return ITALIC_VARIANT;
    case T_BOLD_ITALIC: return BOLD_ITALIC_VARIANT;
    case T_DOUBLE_STRUCK: return DOUBLE_STRUCK_VARIANT;
    case T_BOLD_FRAKTUR: return BOLD_FRAKTUR_VARIANT;
    case T_SCRIPT: return SCRIPT_VARIANT;
    case T_BOLD_SCRIPT: return BOLD_SCRIPT_VARIANT;
    case T_FRAKTUR: return FRAKTUR_VARIANT;
    case T_SANS_SERIF: return SANS_SERIF_VARIANT;
    case T_BOLD_SANS_SERIF: return BOLD_SANS_SERIF_VARIANT;
    case T_SANS_SERIF_ITALIC: return SANS_SERIF_ITALIC_VARIANT;
    case T_SANS_SERIF_BOLD_ITALIC: return SANS_SERIF_BOLD_ITALIC_VARIANT;
    case T_MONOSPACE: return MONOSPACE_VARIANT;
    default: assert(false);
    }
}

MathVariant
toMathVariant(const SmartPtr<Value>& value)
{
  return toMathVariant(ToTokenId(value));
}

FormattingContext::PropertyId
toMathSpaceId(TokenId id)
{
  switch (id)
    {
    case T_NEGATIVEVERYVERYTHICKMATHSPACE: return FormattingContext::NEGATIVE_VERYVERYTHICK_SPACE;
    case T_NEGATIVEVERYTHICKMATHSPACE: return FormattingContext::NEGATIVE_VERYTHICK_SPACE;
    case T_NEGATIVETHICKMATHSPACE: return FormattingContext::NEGATIVE_THICK_SPACE;
    case T_NEGATIVEMEDIUMMATHSPACE: return FormattingContext::NEGATIVE_MEDIUM_SPACE;
    case T_NEGATIVETHINMATHSPACE: return FormattingContext::NEGATIVE_THIN_SPACE;
    case T_NEGATIVEVERYTHINMATHSPACE: return FormattingContext::NEGATIVE_VERYTHIN_SPACE;
    case T_NEGATIVEVERYVERYTHINMATHSPACE: return FormattingContext::NEGATIVE_VERYVERYTHIN_SPACE;
    case T_VERYVERYTHINMATHSPACE: return FormattingContext::VERYVERYTHIN_SPACE;
    case T_VERYTHINMATHSPACE: return FormattingContext::VERYTHIN_SPACE;
    case T_THINMATHSPACE: return FormattingContext::THIN_SPACE;
    case T_MEDIUMMATHSPACE: return FormattingContext::MEDIUM_SPACE;
    case T_THICKMATHSPACE: return FormattingContext::THICK_SPACE;
    case T_VERYTHICKMATHSPACE: return FormattingContext::VERYTHICK_SPACE;
    case T_VERYVERYTHICKMATHSPACE: return FormattingContext::VERYVERYTHICK_SPACE;
    default:
      assert(false);
      return FormattingContext::ZERO_SPACE;
    }
}

Length
toLength(const SmartPtr<Value>& value, const FormattingContext& ctxt)
{
  if (IsTokenId(value))
    return ctxt.getMathSpace(toMathSpaceId(ToTokenId(value)));
  else
    return ToLength(value);
}

Length
resolveLength(const FormattingContext& ctxt, const SmartPtr<Value>& value, int i, int j)
{ return toLength(GetComponent(value, i, j), ctxt); }
