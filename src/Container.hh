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

#ifndef Container_hh
#define Container_hh

template <class Type>
class Iterator;

template <class Type>
class Container {
public:
  Container(void);
  Container(const Container&);
  ~Container();

  unsigned GetSize(void) const { return n; }
  bool 	   IsEmpty(void) const { return GetSize() == 0; }
  void 	   AddFirst(Type);
  void 	   AddLast(Type);
  void 	   Append(Type t) { AddLast(t); }
  Type 	   RemoveFirst(void);
  Type 	   RemoveLast(void);

  bool 	   Contains(Type) const;
  void 	   Remove(Type);
  void 	   Substitute(Type, Type);
  int      IndexOf(Type) const;

  Type     Get(unsigned) const;
  Type 	   GetFirst(void) const;
  Type 	   GetLast(void) const;

  friend class Iterator<Type>;

private:
  struct Wrapper {
    Type     obj;
    Wrapper* next;
    Wrapper* prev;
  };

  Wrapper* Search(const Type, int* = NULL) const;

  unsigned n;
  Wrapper* first;
  Wrapper* last;
};

#include "Container.icc"

#endif // Container_hh
