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
#include <stdio.h>

#include "Value.hh"
#include "ValueSequence.hh"

#ifdef DEBUG
#  define DEBUGVALUE counter++
int Value::counter = 0;
int Value::cached = 0;
#else
#  define DEBUGVALUE
#endif

Value* Value::firstFree = NULL;

void*
Value::operator new(size_t size)
{
  if (firstFree == NULL)
    return reinterpret_cast<Value*>(new char[size]);
  else {
    Value* v = firstFree;
    firstFree = v->next;
    return v;
  }
}

void Value::operator delete(void* p)
{
  assert(p != NULL);
  (static_cast<Value*>(p))->next = firstFree;
  firstFree = static_cast<Value*>(p);
#ifdef DEBUG
  counter--;
#endif
}

void
Value::operator delete(void* p, size_t)
{
  assert(p != NULL);
  (static_cast<Value*>(p))->next = firstFree;
  firstFree = static_cast<Value*>(p);
#ifdef DEBUG
  counter--;
#endif
}

void
Value::Flush()
{
  Value* v = firstFree;

  while (v != NULL) {
    Value* next = v->next;
    ::delete v;
    v = next;
  }

  firstFree = NULL;
}

#ifdef DEBUG
void
Value::AddCached()
{
  cached++;
}

void
Value::RemoveCached()
{
  cached--;
}
#endif // DEBUG

Value::Value()
{
  type = VALUE_EMPTY;
  DEBUGVALUE;
}

Value::Value(bool v)
{
  SetBoolean(v);
  DEBUGVALUE;
}

Value::Value(int v)
{
  SetInteger(v);
  DEBUGVALUE;
}

Value::Value(float v)
{
  SetNumber(v);
  DEBUGVALUE;
}

Value::Value(UnitId v)
{
  SetUnit(v);
  DEBUGVALUE;
}

Value::Value(KeywordId v)
{
  SetKeyword(v);
  DEBUGVALUE;
}

Value::Value(const UnitValue& v)
{
  SetNumberUnit(v);
  DEBUGVALUE;
}

Value::Value(float d, UnitId v)
{
  UnitValue value;
  value.Set(d, v);
  SetNumberUnit(value);
  DEBUGVALUE;
}

Value::Value(const String* v)
{
  SetString(v);
  DEBUGVALUE;
}

Value::Value(const ValueSequence* v)
{
  SetSequence(v);
  DEBUGVALUE;
}

Value::Value(const Value& value)
{
  *this = value;
  switch (type) {
  case VALUE_STRING:
    if (stringVal != NULL) stringVal = stringVal->Clone();
    break;
  case VALUE_SEQUENCE:
    if (seqVal != NULL) seqVal = new ValueSequence(*seqVal);
    break;
  default:
    break;
  }
  DEBUGVALUE;
}

Value::~Value()
{
  switch (type) {
  case VALUE_STRING: delete stringVal; break;
  case VALUE_SEQUENCE: delete seqVal; break;
  default: break;
  }

  type = VALUE_EMPTY;
}

Char Value::ToChar() const
{
  assert(IsChar());
  return charVal;
}

bool Value::ToBoolean() const
{
  assert(IsBoolean());
       if (IsKeyword(KW_TRUE))  return true;
  else if (IsKeyword(KW_FALSE)) return false;
  else                          return boolVal;
}

int Value::ToInteger() const
{
  assert(IsInteger());
  return intVal;
}

UnitId Value::ToUnit() const
{
  assert(IsUnit());
  return unitVal;
}

KeywordId Value::ToKeyword() const
{
  assert(IsKeyword());
  return keywordVal;
}

float Value::ToNumber() const
{
  assert(IsNumber());
  return numberVal;
}

const UnitValue& Value::ToNumberUnit() const
{
  assert(IsNumberUnit());
  return numberUnitVal;
}

RGBValue Value::ToRGB() const
{
  assert(IsRGB());
  return rgbVal;
}

const String* Value::ToString() const
{
  assert(IsString());
  return stringVal;
}

const ValueSequence* Value::ToValueSequence() const
{
  assert(IsSequence());
  return seqVal;
}

void Value::SetChar(Char v)
{
  type = VALUE_CHAR;
  charVal = v;
}

void Value::SetBoolean(bool v)
{
  type = VALUE_BOOLEAN;
  boolVal = v;
}

void Value::SetInteger(int v)
{
  type = VALUE_INTEGER;
  intVal = v;
}

void Value::SetUnit(UnitId v)
{
  type = VALUE_UNIT;
  unitVal = v;
}

void Value::SetKeyword(KeywordId v)
{
  type = VALUE_KEYWORD;
  keywordVal = v;
}

void Value::SetRGB(RGBValue v)
{
  type = VALUE_RGB;
  rgbVal = v;
}

void Value::SetNumber(float v)
{
  type = VALUE_NUMBER;
  numberVal = v;
}

void Value::SetNumberUnit(const UnitValue& v)
{
  type = VALUE_NUMBERUNIT;
  numberUnitVal = v;
}

void Value::SetString(const String* v)
{
  type = VALUE_STRING;
  stringVal = v;
}

void Value::SetSequence(const ValueSequence* v)
{
  assert(v != NULL);
  type = VALUE_SEQUENCE;
  seqVal = v;
}

bool Value::IsResolved() const
{
  return !IsNumberUnit() || !numberUnitVal.IsPercentage();
}

const Value* Value::Get(int i, int j) const
{
  if (i < 0) return this;
  else {
    assert(IsSequence());
    const ValueSequence* sequence = ToValueSequence();
    assert(sequence != NULL && !sequence->IsEmpty());
    return sequence->GetLastValue(i)->Get(j, -1);
  }  
}

void Value::Dump() const
{
  switch (type) {
  case VALUE_EMPTY:
    printf("<empty>");
    break;
  case VALUE_CHAR:
    if (isPlain(charVal)) printf("'%c'", charVal);
    else printf("U+%04x", charVal);
    break;
  case VALUE_BOOLEAN:
    printf("%s", boolVal ? "true" : "false");
    break;
  case VALUE_INTEGER:
    printf("%d", intVal);
    break;
  case VALUE_NUMBER:
    printf("%f", numberVal);
    break;
  case VALUE_RGB:
    printf("RGB:%06x", rgbVal);
    break;
  case VALUE_UNIT:
    printf("%s", NameOfUnitId(unitVal));
    break;
  case VALUE_NUMBERUNIT:
    printf("%f%s", numberUnitVal.GetValue(), NameOfUnitId(numberUnitVal.GetUnitId()));
    break;
  case VALUE_STRING:
    assert(stringVal != NULL);
    printf("\"%s\"", stringVal->ToStaticC());
    break;
  case VALUE_KEYWORD:
    printf("%s", NameOfKeywordId(keywordVal));
    break;
  case VALUE_SEQUENCE:
    assert(seqVal != NULL);
    putchar('[');
    seqVal->Dump();
    printf(" ]");
    break;
  default:
    putchar('?');
    break;
  }
}
