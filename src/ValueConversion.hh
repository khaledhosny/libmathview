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
#include "UnitValue.hh"
#include "Value.hh"
#include "Variant.hh"
#include "keyword.hh"
#include "RGBValue.hh"

bool            IsEmpty(const SmartPtr<Value>&);
bool            IsKeyword(const SmartPtr<Value>&);
bool            IsSequence(const SmartPtr<Value>&);
bool            IsNumberUnit(const SmartPtr<Value>&);
bool            IsRGBValue(const SmartPtr<Value>&);

bool            ToBoolean(const SmartPtr<Value>&);
int             ToInteger(const SmartPtr<Value>&);
float           ToNumber(const SmartPtr<Value>&);
class String*   ToString(const SmartPtr<Value>&);
SmartPtr<ValueSequence> ToSequence(const SmartPtr<Value>&);
RGBValue        ToRGBValue(const SmartPtr<Value>&);
KeywordId       ToKeywordId(const SmartPtr<Value>&);
RowAlignId      ToRowAlignId(const SmartPtr<Value>&);
ColumnAlignId   ToColumnAlignId(const SmartPtr<Value>&);
GroupAlignId 	ToGroupAlignId(const SmartPtr<Value>&);
TableLineId  	ToLineId(const SmartPtr<Value>&);
MathSpaceId    	ToNamedSpaceId(const SmartPtr<Value>&);
UnitId         	ToUnitId(const SmartPtr<Value>&);
FontWeightId 	ToFontWeightId(const SmartPtr<Value>&);
FontStyleId  	ToFontStyleId(const SmartPtr<Value>&);
RGBValue      	ToRGB(const SmartPtr<Value>&);
OperatorFormId  ToFormId(const SmartPtr<Value>&);
FractionAlignId ToFractionAlignId(const SmartPtr<Value>&);
MarkAlignType   ToMarkAlignId(const SmartPtr<Value>&);
UnitValue       ToNumberUnit(const SmartPtr<Value>&);
BreakId         ToBreakId(const SmartPtr<Value>&);

SmartPtr<Value> GetComponent(const SmartPtr<Value>&, int = -1, int = -1);

#endif // __ValueConversion_hh__
