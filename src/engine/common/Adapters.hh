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

#ifndef __Adapters_hh__
#define __Adapters_hh__

#include <functional>

#include "SmartPtr.hh"

template <typename T, typename TPtr = SmartPtr<T> >
struct ConstructAdapter
  : public std::unary_function<TPtr,void>
{
  void operator()(const TPtr& elem) const
  { elem->construct(); }
};

template <typename C, typename T, typename TPtr = SmartPtr<T> >
struct RefineAdapter
  : public std::binary_function<TPtr,C*,void>
{
  void operator()(const TPtr& elem, C* context) const
  { elem->refine(*context); }
};

template <typename C, typename T, typename R, typename TPtr = SmartPtr<T> >
struct FormatAdapter
  : public std::binary_function<TPtr,C*,R>
{
  R operator()(const TPtr& elem, C* context) const
  { return elem->format(*context); }
};

template <typename T, typename TPtr = SmartPtr<T> >
struct SetFlagDownAdapter
  : public std::binary_function<TPtr, typename T::Flags,void>
{
  void operator()(const TPtr& elem, typename T::Flags f) const
  { elem->setFlagDown(f); }
};

template <typename T, typename TPtr = SmartPtr<T> >
struct ResetFlagDownAdapter
  : public std::binary_function<TPtr, typename T::Flags,void>
{
  void operator()(const TPtr& elem, typename T::Flags f) const
  { elem->resetFlagDown(f); }
};

template <typename T, typename TPtr = SmartPtr<T> >
struct NotNullPredicate
  : public std::unary_function<TPtr,bool>
{
  bool operator()(const TPtr& elem) const
  { return elem; }
};

#endif // __Adapters_hh__
