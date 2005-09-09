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

#ifndef __Variant_hh__
#define __Variant_hh__

#include <vector>

#include "SmartPtr.hh"
#include "Value.hh"

template <typename T>
class GMV_MathView_EXPORT Variant : public Value
{
protected:
  Variant(const T& v) : value(v) { }
  virtual ~Variant() { }

public:
  static SmartPtr<Variant> create(const T& v) { return new Variant(v); }
  T getValue(void) const { return value; }

private:
  T value;
};

template <>
class GMV_MathView_EXPORT Variant<void> : public Value
{
protected:
  Variant(void) { }
  virtual ~Variant() { }

public:
  static SmartPtr< Variant<void> > create(void)
  { return new Variant(); }
};

// The following full specialization is for sequences of Values
// There are more accessor methods and getValue returns a
// const reference instead of a copy fo the object
template <>
class GMV_MathView_EXPORT Variant< std::vector< SmartPtr<Value> > > : public Value
{
protected:
  Variant(const std::vector< SmartPtr<Value> >& v) : content(v) { }
  virtual ~Variant() { }

public:
  static SmartPtr< Variant< std::vector< SmartPtr<Value> > > > create(const std::vector< SmartPtr<Value> >& v)
  { return new Variant(v); }
  const std::vector< SmartPtr<Value> >& getValue(void) const { return content; }
  unsigned getSize(void) const { return content.size(); }
  SmartPtr<Value> getValue(unsigned i) const { return content[i]; }

private:
  std::vector< SmartPtr<Value> > content;
};

typedef Variant< std::vector< SmartPtr<Value> > > ValueSequence;

template <typename T>
/* GMV_MathView_EXPORT */ T
as(const Value* v)
{
  if (const Variant<T>* obj = dynamic_cast<const Variant<T>*>(v))
    return obj->getValue();
  else
    throw Value::TypeError();
}

#endif // __Variant_hh__
