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
// http://www.cs.unibo.it/helm/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef Ptr_hh
#define Ptr_hh

#include <cassert>

template <class P>
class Ptr
{
public:
  Ptr(P* p = 0) : ptr(p) { if (ptr) ptr->ref(); }
  Ptr(const Ptr& p) : ptr(p.ptr) { if (ptr) ptr->ref(); }
  ~Ptr() { if (ptr) ptr->unref(); }

  P* operator->() const { assert(ptr); return ptr; }
  Ptr& operator=(const Ptr& p)
  { 
    if (ptr == p.ptr) return *this;
    if (p.ptr) p.ptr->ref();
    if (ptr) ptr->unref();
    ptr = p.ptr;
    return *this;
  }

  operator P*() const { return ptr; }
  template <class Q> friend Ptr<Q> smart_cast(const Ptr& p) { return Ptr<Q>(dynamic_cast<Q*>(p.ptr)); }  
  template <class Q> friend bool is_a(const Ptr& p) { return dynamic_cast<Q*>(p.ptr) != 0; }
  template <class Q> operator Ptr<Q>() const { return Ptr<Q>(ptr); }

private:
  P* ptr;
};

#endif // Ptr_hh
