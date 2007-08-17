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

#ifndef __TokenSet_hh__
#define __TokenSet_hh__

#include "token.hh"

template <TokenId id>
class Singleton
{
public:
  static inline bool
  empty(void)
  { return false; }

  static inline bool
  has(TokenId kw)
  { return kw == id; }
};

template <typename S1, typename S2>
class Union
{
public:
  static inline bool
  empty(void)
  { return S1::empty() && S2::empty(); }

  static inline bool
  has(TokenId kw)
  { return S1::has(kw) || S2::has(kw); }
};

template <TokenId k1 = T__NOTVALID,
	  TokenId k2 = T__NOTVALID,
	  TokenId k3 = T__NOTVALID,
	  TokenId k4 = T__NOTVALID,
	  TokenId k5 = T__NOTVALID,
	  TokenId k6 = T__NOTVALID,
	  TokenId k7 = T__NOTVALID,
	  TokenId k8 = T__NOTVALID,
	  TokenId k9 = T__NOTVALID,
	  TokenId k10 = T__NOTVALID,
	  TokenId k11 = T__NOTVALID,
	  TokenId k12 = T__NOTVALID,
	  TokenId k13 = T__NOTVALID,
	  TokenId k14 = T__NOTVALID>
class Set
{
public:
  static inline bool
  empty(void)
  { return k1 == T__NOTVALID; }

  static inline bool
  has(TokenId kw)
  { return empty() ? false : Singleton<k1>::has(kw) || Set<k2,k3,k4,k5,k6,k7,k8,k9,k10,k11,k12,k13,k14>::has(kw); }
};

#endif // __TokenSet_hh__
