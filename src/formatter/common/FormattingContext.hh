// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#ifndef __FormattingContext_hh__
#define __FormattingContext_hh__

#include <string>

#include "Variant.hh"
#include "ScopedHashMap.hh"

class FormattingContext
{
public:
  FormattingContext(void) { }

  template <typename T>
  void set(const char* id, const T& v)
  { map.set(id, Variant<T>::create(v)); }

  template <typename T>
  T get(const char* id) const
  { return as<T>(map.get(id)); }

  bool defined(const char* id) const
  { return map.defined(id); }

protected:
  template <typename T>
  void set(int id, const T& v)
  { fmap.set(id, Variant<T>::create(v)); }

  template <typename T>
  T get(int id) const
  { return as<T>(fmap.get(id)); }

public:
  void push(void)
  {
    fmap.push();
    map.push();
  }

  void pop()
  {
    fmap.pop();
    map.pop();
  }

private:
  ScopedHashMap<int, SmartPtr<Value> > fmap;
  ScopedHashMap<const char*, SmartPtr<Value> > map;
};

#endif // __FormattingContext_hh__
