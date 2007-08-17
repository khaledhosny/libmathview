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

#ifndef __Model_hh__
#define __Model_hh__

#include "libxml2_Model.hh"

struct Model : libxml2_Model
{
  static String getAttributeNS(const Element&, const String&, const String&);
  static String getAttributeNoNS(const Element&, const String&);
  static void setAttribute(const Element&, const String&, const String&);
  static bool setAttributeNS(const Element&, const String&, const String&, const String&);
};

#endif // __Model_hh__
