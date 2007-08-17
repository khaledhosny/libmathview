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

#ifndef __GObjectPtr_hh__
#define __GObjectPtr_hh__

template <class P>
class GObjectPtr
{
public:
  GObjectPtr(P* p = 0) : ptr(p) { if (ptr) g_object_ref(ptr); }
  GObjectPtr(const GObjectPtr& p) : ptr(p.ptr) { if (ptr) g_object_ref(ptr); }
  ~GObjectPtr() { if (ptr) g_object_unref(ptr); }

  GObjectPtr& operator=(const GObjectPtr& p)
  { 
    if (ptr == p.ptr) return *this;
    if (p.ptr) g_object_ref(p.ptr);
    if (ptr) g_object_unref(ptr);
    ptr = p.ptr;
    return *this;
  }

  operator P*() const { return ptr; }

private:
  P* ptr;
};

#endif // __GObjectPtr_hh__
