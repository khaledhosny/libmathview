// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#ifndef __Scanner_hh__
#define __Scanner_hh__

#include "String.hh"
#include "Value.hh"
#include "SmartPtr.hh"

class Scanner
{
public:
  Scanner(const UCS4String&, bool = true);
  ~Scanner();

  enum TokenId
    {
      ERROR = -1,
      UNDEFINED,
      EOS,
      RAW,
      NUMBER,
      ID,
      LPAREN,
      RPAREN,
      LBRACE,
      RBRACE,
      SHARP,
      DOLLAR,
      COMMA
    };

  bool more(void) const { return p != end; }
  TokenId getToken(void);
  void advance(void);

  UCS4String getString(void) const;
  float getNumber(void) const;
  
protected:
  TokenId scanToken(void);

private:
  bool raw;
  TokenId token;
  SmartPtr<Value> value;
  UCS4String buffer;
  UCS4String::const_iterator p;
  UCS4String::const_iterator end;
};

#endif // __Scanner_hh__
