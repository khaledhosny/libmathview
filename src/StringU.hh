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

#ifndef StringU_hh
#define StringU_hh

#include "String.hh"
#include "Char.hh"

template <typename CharX>
class StringU : public String
{
public:
  StringU(void);
  StringU(unsigned);
  StringU(const char*);
  StringU(const Char8*, unsigned);
  StringU(const Char16*, unsigned);
  StringU(const Char32*, unsigned);
  StringU(const String&);
  StringU(const String&, unsigned, unsigned);
  virtual ~StringU();

  virtual String* Clone(void) const;

  virtual void TrimSpacesLeft(void);
  virtual void TrimSpacesRight(void);
  virtual void CollapseSpaces(void);
  virtual void DeleteSpaces(void);
  virtual void Append(const String&);

  virtual void Set(const char*);
  virtual void Set(const Char8*, unsigned);
  virtual void Set(const Char16*, unsigned);
  virtual void Set(const Char32*, unsigned);
  virtual void Set(const String&);
  virtual void Set(const String&, unsigned, unsigned);
  virtual void SetChar(unsigned i, Char ch);

  virtual unsigned GetLength(void) const;
  virtual void SetLength(unsigned);
  virtual Char GetChar(unsigned i) const;

  const CharX* GetData(void) const { return data; }

protected:
  void Init(unsigned = 0);
  void Delete(void);

private:
  unsigned length;
  CharX*   data;
};

#endif // StringU_hh
