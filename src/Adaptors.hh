// Copyright (C) 2000-2002, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#ifndef __Adaptors_hh__
#define __Adaptors_hh__

#include <functional>

#include "SmartPtr.hh"
#include "MathMLElement.hh"

struct ConstructAdaptor
  : public std::unary_function<SmartPtr<MathMLElement>,void>
{
  void operator()(const SmartPtr<MathMLElement>& elem) const
  { elem->construct(); }
};

struct RefineAdaptor
  : public std::binary_function<SmartPtr<MathMLElement>,class AbstractRefinementContext*,void>
{
  void operator()(const SmartPtr<MathMLElement>& elem, class AbstractRefinementContext* context) const
  { elem->refine(*context); }
};

#if 0
struct SetupAdaptor
  : public std::binary_function<SmartPtr<MathMLElement>,class RenderingEnvironment*,void>
{
  void operator()(const SmartPtr<MathMLElement>& elem, class RenderingEnvironment* env) const
  { elem->Setup(*env); }
};

struct DoLayoutAdaptor
  : public std::binary_function<SmartPtr<MathMLElement>,const class FormattingContext*,void>
{
  void operator()(const SmartPtr<MathMLElement>& elem, const class FormattingContext* ctxt) const
  { elem->DoLayout(*ctxt); }
};
#endif

struct SetFlagDownAdaptor
  : public std::binary_function<SmartPtr<MathMLElement>,MathMLElement::Flags,void>
{
  void operator()(const SmartPtr<MathMLElement>& elem, MathMLElement::Flags f) const
  { elem->SetFlagDown(f); }
};

struct ResetFlagDownAdaptor
  : public std::binary_function<SmartPtr<MathMLElement>,MathMLElement::Flags,void>
{
  void operator()(const SmartPtr<MathMLElement>& elem, MathMLElement::Flags f) const
  { elem->ResetFlagDown(f); }
};

#if 0
struct SetDirtyAdaptor
  : public std::binary_function<SmartPtr<MathMLElement>,const class Rectangle*,void>
{
  void operator()(const SmartPtr<MathMLElement>& elem, const class Rectangle* rect) const
  { elem->SetDirty(rect); }
};

struct SetDirtyLayoutAdaptor
  : public std::binary_function<SmartPtr<MathMLElement>,bool,void>
{
  void operator()(const SmartPtr<MathMLElement>& elem, bool children) const
  { elem->SetDirtyLayout(children); }
};

// WARNING: we have to use const DrawingArea* instead of
// const DrawingArea& because otherwise the compiler tries to
// make a reference to reference, which is forbidden.
// Is it possible that the STL developers did not foresee this?
struct RenderAdaptor
  : public std::binary_function<SmartPtr<MathMLElement>,const class DrawingArea*,void>
{
  void operator()(const SmartPtr<MathMLElement>& elem, const class DrawingArea* area) const
  { elem->Render(*area); }
};

struct SetSelectedAdaptor
  : public std::unary_function<SmartPtr<MathMLElement>,void>
{
  void operator()(const SmartPtr<MathMLElement>& elem) const
  { elem->SetSelected(); }
};

struct ResetSelectedAdaptor
  : public std::unary_function<SmartPtr<MathMLElement>,void>
{
  void operator()(const SmartPtr<MathMLElement>& elem) const
  { elem->ResetSelected(); }
};

struct ReleaseGCsAdaptor
  : public std::unary_function<SmartPtr<MathMLElement>,void>
{
  void operator()(const SmartPtr<MathMLElement>& elem) const
  { elem->ReleaseGCs(); }
};
#endif

struct IsSpaceLikePredicate
  : public std::unary_function<SmartPtr<MathMLElement>,bool>
{
  bool operator()(const SmartPtr<MathMLElement>& elem) const
  { return elem->IsSpaceLike(); }
};

struct NotNullPredicate
  : public std::unary_function<SmartPtr<MathMLElement>,bool>
{
  bool operator()(const SmartPtr<MathMLElement>& elem) const
  { return elem; }
};

#endif // __Adaptors_hh__
