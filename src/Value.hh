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

#ifndef Value_hh
#define Value_hh

#include "String.hh"
#include "keyword.hh"
#include "UnitValue.hh"
#include "RGBValue.hh"

enum ValueType {
  // Note: when a value is optional and not given, it is an Empty-tagged Value
  VALUE_EMPTY,
  
  VALUE_CHAR,
  VALUE_BOOLEAN,
  VALUE_INTEGER,
  VALUE_NUMBER,
  VALUE_RGB,
  VALUE_UNIT,
  VALUE_NUMBERUNIT,
  VALUE_STRING,
  VALUE_KEYWORD,
  VALUE_SEQUENCE
};

class ValueSequence;
  
class Value {
public:
  void* operator new(size_t);
  // create a matching delete operator
  void operator delete(void*);
  void operator delete(void*, size_t);
  static void Flush(void);
#ifdef DEBUG
  static void AddCached(void);
  static void RemoveCached(void);
#endif

  Value(void);
  Value(bool);
  Value(int);
  Value(UnitId);
  Value(float);
  Value(KeywordId);
  Value(const UnitValue&);
  Value(float, UnitId);
  Value(const String*);
  Value(const ValueSequence*);
  Value(const Value&);
  ~Value();

  Char 	    ToChar(void) const;
  UnitId    ToUnit(void) const;
  bool 	    ToBoolean(void) const;
  int       ToInteger(void) const;
  float     ToNumber(void) const;
  KeywordId ToKeyword(void) const;
  RGBValue  ToRGB(void) const;
  const     UnitValue& ToNumberUnit(void) const;
  const String* ToString(void) const;
  const ValueSequence* ToValueSequence(void) const;

  void 	    SetChar(Char);
  void      SetUnit(UnitId);
  void 	    SetBoolean(bool);
  void 	    SetInteger(int);
  void 	    SetNumber(float);
  void 	    SetKeyword(KeywordId);
  void 	    SetRGB(RGBValue);
  void 	    SetNumberUnit(const UnitValue&);
  void      SetString(const String*);
  void 	    SetSequence(const ValueSequence*);
   
  ValueType IsA(void) const { return type; }
  bool      IsResolved(void) const;
  bool 	    IsEmpty(void) const { return type == VALUE_EMPTY; }
  bool 	    IsChar(void) const { return type == VALUE_CHAR; }
  bool      IsUnit(void) const { return type == VALUE_UNIT; }
  bool 	    IsBoolean(void) const {
    return type == VALUE_BOOLEAN || IsKeyword(KW_TRUE) || IsKeyword(KW_FALSE);
  }
  bool 	    IsInteger(void) const { return type == VALUE_INTEGER; }
  bool 	    IsRGB(void) const { return type == VALUE_RGB; }
  bool 	    IsNumber(void) const { return type == VALUE_NUMBER; }
  bool 	    IsNumberUnit(void) const { return type == VALUE_NUMBERUNIT; }
  bool 	    IsString(void) const { return type == VALUE_STRING; }
  bool 	    IsKeyword(void) const { return type == VALUE_KEYWORD; }
  bool 	    IsKeyword(KeywordId id) const {
    return type == VALUE_KEYWORD && keywordVal == id;
  }
  bool 	    IsSequence(void) const { return type == VALUE_SEQUENCE; }

  void      Dump(void) const;

  // list facilities
  const Value* Get(int = -1, int = -1) const;

#ifdef DEBUG
  static int GetCounter(void) { return counter; }
  static int GetCached(void) { return cached; }
#endif

private:
  ValueType type;
  union {
    Char                 charVal;
    bool                 boolVal;
    int                  intVal;
    float                numberVal;
    UnitId               unitVal;
    KeywordId            keywordVal;
    UnitValue            numberUnitVal;
    RGBValue             rgbVal;
    const String*        stringVal;
    const ValueSequence* seqVal;
    Value*               next;
  };

#ifdef DEBUG
  static int counter;
  static int cached;
#endif

  static Value* firstFree;
};

typedef const Value* ValuePtr;

#endif // Value_hh
