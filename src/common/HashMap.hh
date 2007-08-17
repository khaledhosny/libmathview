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

#ifndef __HashMap_hh__
#define __HashMap_hh__

// !!! BEGIN WARNING: hash_map is not part of the STL !!!
#if defined(GMV_HAVE_EXT_HASH_MAP)
  #include <ext/hash_map>
#define HASH_MAP_NS __gnu_cxx
#elif defined(GMV_HAVE_HASH_MAP)
  #include <hash_map>
#define HASH_MAP_NS std
#else
#error "no implementation of hash_map could be found"
#endif
// !!! END WARNING: hash_map is not part of the STL !!!

#endif // __HashMap_hh__
