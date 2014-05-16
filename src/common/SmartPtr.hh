// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

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

template <class Q, class P>
SmartPtr<Q>
smart_cast(const SmartPtr<P>& p)
{ return SmartPtr<Q>(dynamic_cast<Q*>(p.ptr)); }  

template <class Q, class R>
bool
is_a(const SmartPtr<R>& p)
{ return dynamic_cast<Q*>(p.ptr) != 0; }

#endif // __SmartPtr_hh__
