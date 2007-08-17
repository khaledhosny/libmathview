// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __WeakPtr_hh__
#define __WeakPtr_hh__

#include <cassert>

#include "gmv_defines.h"

template <class P>
class GMV_MathView_EXPORT WeakPtr
{
public:
  WeakPtr(P* p = 0) : ptr(p) { }
  WeakPtr(const WeakPtr& p) : ptr(p.ptr) { }
  ~WeakPtr() { }

  P* operator->() const { assert(ptr); return ptr; }
  WeakPtr& operator=(const WeakPtr& p)
  { 
    ptr = p.ptr;
    return *this;
  }

  operator P*() const { return ptr; }
  template <class Q> friend WeakPtr<Q> weak_cast(const WeakPtr& p) { return WeakPtr<Q>(dynamic_cast<Q*>(p.ptr)); }  
  template <class Q> operator WeakPtr<Q>() const { return WeakPtr<Q>(ptr); }

private:
  P* ptr;
};

#endif // __WeakPtr_hh__
