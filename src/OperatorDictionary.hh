// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef OperatorDictionary_hh
#define OperatorDictionary_hh

#include <vector>
// !!! BEGIN WARNING: hash_map is not part of the STL !!!
#if defined(HAVE_EXT_HASH_MAP)
#include <ext/hash_map>
#elif defined(HAVE_HASH_MAP)
#include <hash_map>
#else
#error "no implementation of hash_map could be found"
#endif
// !!! END WARNING: hash_map is not part of the STL !!!

#include "String.hh"

class OperatorDictionary
{
public:
  OperatorDictionary(void);
  ~OperatorDictionary();

  bool Load(const char*);
  void Unload(void);
  void Search(const String*,
	      const class MathMLAttributeList**,
	      const class MathMLAttributeList**,
	      const class MathMLAttributeList**) const;

private:
  struct FormDefaults
  {
    FormDefaults(void) { prefix = infix = postfix = 0; };
    ~FormDefaults();
    const class MathMLAttributeList* prefix;
    const class MathMLAttributeList* infix;
    const class MathMLAttributeList* postfix;
  };

  void Delete(void);

#if defined(HAVE_EXT_HASH_MAP)
  typedef __gnu_cxx::hash_map<const String*, FormDefaults, String::Hash, String::Eq> Dictionary;
#elif defined(HAVE_HASH_MAP)
  typedef std::hash_map<const String*, FormDefaults, String::Hash, String::Eq> Dictionary;
#endif
  Dictionary items;
};

#endif // OperatorDictionary_hh
