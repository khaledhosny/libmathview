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

#include "Ptr.hh"
#include "MathMLElement.hh"

struct NormalizeAdaptor
  : public std::binary_function< Ptr<MathMLElement>,Ptr<class MathMLDocument>,void>
{
  void operator()(const Ptr<MathMLElement>& elem, const Ptr<MathMLDocument>& doc) const
  { elem->Normalize(doc); }
};

struct SetupAdaptor
  : public std::binary_function<Ptr<MathMLElement>,class RenderingEnvironment*,void>
{
  void operator()(const Ptr<MathMLElement>& elem, class RenderingEnvironment* env) const
  { elem->Setup(*env); }
};

struct DoLayoutAdaptor
  : public std::binary_function<Ptr<MathMLElement>,const class FormattingContext*,void>
{
  void operator()(const Ptr<MathMLElement>& elem, const class FormattingContext* ctxt) const
  { elem->DoLayout(*ctxt); }
};

struct SetFlagDownAdaptor
  : public std::binary_function<Ptr<MathMLElement>,MathMLElement::Flags,void>
{
  void operator()(const Ptr<MathMLElement>& elem, MathMLElement::Flags f) const
  { elem->SetFlagDown(f); }
};

struct ResetFlagDownAdaptor
  : public std::binary_function<Ptr<MathMLElement>,MathMLElement::Flags,void>
{
  void operator()(const Ptr<MathMLElement>& elem, MathMLElement::Flags f) const
  { elem->ResetFlagDown(f); }
};

#if 0
struct SetDirtyAdaptor
  : public std::binary_function<Ptr<MathMLElement>,const class Rectangle*,void>
{
  void operator()(const Ptr<MathMLElement>& elem, const class Rectangle* rect) const
  { elem->SetDirty(rect); }
};

struct SetDirtyLayoutAdaptor
  : public std::binary_function<Ptr<MathMLElement>,bool,void>
{
  void operator()(const Ptr<MathMLElement>& elem, bool children) const
  { elem->SetDirtyLayout(children); }
};
#endif

// WARNING: we have to use const DrawingArea* instead of
// const DrawingArea& because otherwise the compiler tries to
// make a reference to reference, which is forbidden.
// Is it possible that the STL developers did not foresee this?
struct RenderAdaptor
  : public std::binary_function<Ptr<MathMLElement>,const class DrawingArea*,void>
{
  void operator()(const Ptr<MathMLElement>& elem, const class DrawingArea* area) const
  { elem->Render(*area); }
};

#if 0
struct SetSelectedAdaptor
  : public std::unary_function<Ptr<MathMLElement>,void>
{
  void operator()(const Ptr<MathMLElement>& elem) const
  { elem->SetSelected(); }
};

struct ResetSelectedAdaptor
  : public std::unary_function<Ptr<MathMLElement>,void>
{
  void operator()(const Ptr<MathMLElement>& elem) const
  { elem->ResetSelected(); }
};
#endif

struct ReleaseGCsAdaptor
  : public std::unary_function<Ptr<MathMLElement>,void>
{
  void operator()(const Ptr<MathMLElement>& elem) const
  { elem->ReleaseGCs(); }
};

struct IsSpaceLikePredicate
  : public std::unary_function<Ptr<MathMLElement>,bool>
{
  bool operator()(const Ptr<MathMLElement>& elem) const
  { return elem->IsSpaceLike(); }
};

struct NotNullPredicate
  : public std::unary_function<Ptr<MathMLElement>,bool>
{
  bool operator()(const Ptr<MathMLElement>& elem) const
  { return elem; }
};

#endif // __Adaptors_hh__
