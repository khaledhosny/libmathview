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

#ifndef __ValueConversion_hh__
#define __ValueConversion_hh__

#include "SmartPtr.hh"
#include "Length.hh"
#include "Value.hh"
#include "Variant.hh"
#include "token.hh"
#include "RGBColor.hh"
#include "String.hh"

bool IsEmpty(const SmartPtr<Value>&);
bool IsTokenId(const SmartPtr<Value>&);
bool IsSequence(const SmartPtr<Value>&);
bool IsLength(const SmartPtr<Value>&);
bool IsRGBColor(const SmartPtr<Value>&);
bool IsNumber(const SmartPtr<Value>&);

bool ToBoolean(const SmartPtr<Value>&);
int ToInteger(const SmartPtr<Value>&);
float ToNumber(const SmartPtr<Value>&);
String ToString(const SmartPtr<Value>&);
SmartPtr<ValueSequence> ToSequence(const SmartPtr<Value>&);
RGBColor ToRGBColor(const SmartPtr<Value>&);
RGBColor ToRGB(const SmartPtr<Value>&);
TokenId ToTokenId(const SmartPtr<Value>&);
Length::Unit toUnitId(TokenId);
Length::Unit toUnitId(const SmartPtr<Value>&);
Length ToLength(const SmartPtr<Value>&);
RGBColor ToRGBColor(const SmartPtr<Value>&);

SmartPtr<Value> GetComponent(const SmartPtr<Value>&, int = -1, int = -1);

#endif // __ValueConversion_hh__
