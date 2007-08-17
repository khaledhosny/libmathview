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

#ifndef __AbstractRefinementContext_hh__
#define __AbstractRefinementContext_hh__

#include "gmetadom.hh"
#include "SmartPtr.hh"
#include "String.hh"

class AbstractRefinementContext
{
protected:
  AbstractRefinementContext(void) { }
  virtual ~AbstractRefinementContext() { }

public:
  virtual SmartPtr<class Attribute> get(const class AttributeSignature&) const = 0;
  virtual void push(const DOM::Element&) = 0;
  virtual void pop(void) = 0;
};

#endif // __AbstractRefinementContext_hh__
