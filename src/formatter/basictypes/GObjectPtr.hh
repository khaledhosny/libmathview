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

#ifndef __GObjectPtr_hh__
#define __GObjectPtr_hh__

template <class P>
class GObjectPtr
{
public:
  GObjectPtr(P* p = 0) : ptr(p) { }
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
