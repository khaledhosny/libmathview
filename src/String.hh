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

#ifndef String_hh
#define String_hh

#include <functional>

#include "defs.h"
#include "Char.hh"

class String
{
public:
  String(void);
  virtual ~String();

  virtual String* Clone(void) const = 0;

  void Dump(void) const;
  char* ToC(void) const;
  const char* ToStaticC(void) const;
  const char* ToTempC(void) const { return ToStaticC(); }
  bool Equal(const String&) const;
  bool Equal(const char*) const;
  virtual void TrimSpacesLeft(void) = 0;
  virtual void TrimSpacesRight(void) = 0;
  virtual void CollapseSpaces(void) = 0;
  virtual void DeleteSpaces(void) = 0;
  virtual void ToLowerCase(void);
  virtual void Append(const String&) = 0;

  virtual void Set(const String&) = 0;
  virtual void Set(const char*) = 0;
  virtual void Set(const Char8*, unsigned) = 0;
  virtual void Set(const Char16*, unsigned) = 0;
  virtual void Set(const Char32*, unsigned) = 0;
  virtual void SetChar(unsigned i, Char ch) = 0;

  virtual unsigned GetLength(void) const = 0;
  virtual void SetLength(unsigned) = 0;
  virtual Char GetChar(unsigned i) const = 0;
  Char operator[](unsigned i) const { return this->GetChar(i); }
  Char GetBiggestChar(void) const;
  Char GetBiggestChar(unsigned, unsigned) const;

#ifdef DEBUG
  static int GetCounter(void) { return counter; }
#endif

  struct Hash : public std::unary_function< class String*, size_t >
  { size_t operator()(const class String*) const; };

  struct Eq : public std::binary_function< class String*, class String*, bool >
  { bool operator()(const class String*, const class String*) const; };

private:
  void ToCAux(char*) const;

#ifdef DEBUG
  static int counter;
#endif
};

typedef String* StringPtr;

#endif // String_hh
