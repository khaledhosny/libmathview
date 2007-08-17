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

#ifndef __ValueConversion_hh__
#define __ValueConversion_hh__

#include "SmartPtr.hh"
#include "Length.hh"
#include "Value.hh"
#include "Variant.hh"
#include "token.hh"
#include "RGBColor.hh"
#include "String.hh"

GMV_MathView_EXPORT bool IsEmpty(const SmartPtr<Value>&);
GMV_MathView_EXPORT bool IsTokenId(const SmartPtr<Value>&);
GMV_MathView_EXPORT bool IsSequence(const SmartPtr<Value>&);
GMV_MathView_EXPORT bool IsLength(const SmartPtr<Value>&);
GMV_MathView_EXPORT bool IsRGBColor(const SmartPtr<Value>&);
GMV_MathView_EXPORT bool IsNumber(const SmartPtr<Value>&);

GMV_MathView_EXPORT bool ToBoolean(const SmartPtr<Value>&);
GMV_MathView_EXPORT int ToInteger(const SmartPtr<Value>&);
GMV_MathView_EXPORT float ToNumber(const SmartPtr<Value>&);
GMV_MathView_EXPORT String ToString(const SmartPtr<Value>&);
GMV_MathView_EXPORT SmartPtr<ValueSequence> ToSequence(const SmartPtr<Value>&);
GMV_MathView_EXPORT RGBColor ToRGBColor(const SmartPtr<Value>&);
GMV_MathView_EXPORT RGBColor ToRGB(const SmartPtr<Value>&);
GMV_MathView_EXPORT TokenId ToTokenId(const SmartPtr<Value>&);
GMV_MathView_EXPORT Length::Unit toUnitId(TokenId);
GMV_MathView_EXPORT Length::Unit toUnitId(const SmartPtr<Value>&);
GMV_MathView_EXPORT Length ToLength(const SmartPtr<Value>&);
GMV_MathView_EXPORT RGBColor ToRGBColor(const SmartPtr<Value>&);

GMV_MathView_EXPORT SmartPtr<Value> GetComponent(const SmartPtr<Value>&, int = -1, int = -1);

#endif // __ValueConversion_hh__
