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

#ifndef __Object_hh__
#define __Object_hh__

class Object
{
protected:
  // Having a protected constructor makes it impossible for users
  // to create directly instances of this class. We want to be sure
  // that all the instances are created dynamically, because of the
  // memory management
  Object(void) : refCounter(0) { }

  // Having a protected destructor makes it impossible for users
  // to call delete on an object of this class. Thus they are
  // forced to use the ref/unref mechanism
  virtual ~Object() { }

public:
  void ref(void) const { refCounter++; }
  void unref(void) const { if (--refCounter == 0) delete this; }

private:
  mutable unsigned refCounter;
};

#endif // __Object_hh__
