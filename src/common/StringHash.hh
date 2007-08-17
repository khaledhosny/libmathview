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

#ifndef __StringHash_hh__
#define __StringHash_hh__

#include <functional>

#include "String.hh"

struct GMV_MathView_EXPORT StringHash : public std::unary_function<String, size_t>
{
  size_t operator()(const String&) const;
};

struct GMV_MathView_EXPORT StringEq : public std::binary_function<String, String, bool>
{
  bool operator()(const String& s1, const String& s2) const
  { return s1 == s2; }
};

#endif // __StringHash_hh__
