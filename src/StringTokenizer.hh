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

#ifndef StringTokenizer_hh
#define StringTokenizer_hh

#include "MathMLAttribute.hh"
#include "UnitValue.hh"
#include "RGBValue.hh"

#define TEMP_BUFFER_SIZE 128

class StringTokenizer
{
public:
  StringTokenizer(const String&);
  ~StringTokenizer();

  void Reset(void);
  bool MoreTokens(void);
  void SkipSpaces(void);
  void SkipToken(void);

  bool Parse(const char*);
  bool Parse(KeywordId);
  bool ParseKeyword(KeywordId*);
  bool ParseString(String*);
  bool ParseUnsignedInteger(int*);
  bool ParseInteger(int*);
  bool ParseUnsignedNumber(float*);
  bool ParseNumber(float*);
  bool ParseChar(Char*);
  bool ParseRGB(RGBValue*);

  unsigned GetMark(void) const { return offset; }
  void SetMark(unsigned);

  const String& GetString(void) const { return s; }

private:
  const char* ParseToken(void);

  const String& s;
  unsigned offset;
};

#endif // StringTokenizer_hh
