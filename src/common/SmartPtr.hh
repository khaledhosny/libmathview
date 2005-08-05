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

#ifndef __SmartPtr_hh__
#define __SmartPtr_hh__

#include <cassert>

template <class P>
class SmartPtr
{
public:
  SmartPtr(P* p = 0) : ptr(p) { if (ptr) ptr->ref(); }
  SmartPtr(const SmartPtr& p) : ptr(p.ptr) { if (ptr) ptr->ref(); }
  ~SmartPtr() { if (ptr) ptr->unref(); }

  P* operator->() const { assert(ptr); return ptr; }
  SmartPtr& operator=(const SmartPtr& p)
  { 
    if (ptr == p.ptr) return *this;
    if (p.ptr) p.ptr->ref();
    if (ptr) ptr->unref();
    ptr = p.ptr;
    return *this;
  }

  operator P*() const { return ptr; }
  template <class Q, class R> friend SmartPtr<Q> smart_cast(const SmartPtr<R>&);
  template <class Q, class R> friend bool is_a(const SmartPtr<R>&);
  template <class Q> operator SmartPtr<Q>() const { return SmartPtr<Q>(ptr); }

private:
  P* ptr;
};

template <class Q, class P> SmartPtr<Q> smart_cast(const SmartPtr<P>& p)
{ return SmartPtr<Q>(dynamic_cast<Q*>(p.ptr)); }  

template <class Q, class R> bool is_a(const SmartPtr<R>& p)
{ return dynamic_cast<Q*>(p.ptr) != 0; }

#endif // __SmartPtr_hh__
