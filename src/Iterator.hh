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

#ifndef Iterator_hh
#define Iterator_hh

#include "Container.hh"

template <class Type>
class Iterator {
public:
  Iterator(const Container<Type>&);
  ~Iterator();

  void ResetFirst(void);
  void ResetLast(void);

  bool More(void) const { return p != 0; }
  bool IsLast(void) const;
  bool IsFirst(void) const;
  void Next(void);
  void Prev(void);

  Type operator()(void) const;

private:
  const Container<Type>& container;
  const Container<Type>::Wrapper* p;
};

template <class Type>
Iterator<Type>::Iterator(const Container<Type>& c) : container(c)
{
  ResetFirst();
}

template <class Type>
Iterator<Type>::~Iterator()
{
}

template <class Type>
inline void Iterator<Type>::ResetFirst()
{
  if (container.IsEmpty()) p = 0;
  else p = container.first;
}

template <class Type>
inline void Iterator<Type>::ResetLast()
{
  if (container.IsEmpty()) p = 0;
  else p = container.last;
}

template <class Type>
inline bool Iterator<Type>::IsFirst() const
{
  return (p != 0) && (p->prev == 0);
}

template <class Type>
inline bool Iterator<Type>::IsLast() const
{
  return (p != 0) && (p->next == 0);
}

template <class Type>
inline void Iterator<Type>::Next()
{
  if (p != 0) p = p->next;
}

template <class Type>
inline void Iterator<Type>::Prev()
{
  if (p != 0) p = p->prev;
}

template <class Type>
inline Type Iterator<Type>::operator()() const
{
  return (p != 0) ? p->obj : 0;
}

#endif

