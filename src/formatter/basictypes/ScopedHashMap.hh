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

#ifndef __ScopedHashMap_hh__
#define __ScopedHashMap_hh__

// !!! BEGIN WARNING: hash_map is not part of the STL !!!
#if defined(HAVE_EXT_HASH_MAP)
#include <ext/hash_map>
#define HASH_MAP_NS __gnu_cxx
#elif defined(HAVE_HASH_MAP)
#include <hash_map>
#define HASH_MAP_NS std
#else
#error "no implementation of hash_map could be found"
#endif
// !!! END WARNING: hash_map is not part of the STL !!!

template <class K, class T, class HashFcn = HASH_MAP_NS::hash<K>, class EqualKey = std::equal_to<K>, class Alloc = std::allocator<T> >
class ScopedHashMap
{
public:
  ScopedHashMap(void) : env(0)
  { push(); }

  ~ScopedHashMap()
  {
    while (env) pop();
    for (Map::iterator p = map.begin(); p != map.end(); p++)
      delete p->second;
  }

  void push(void)
  { env = new Env(env); }

  void pop()
  {
    Entry* p = env->first;
    Env* old_env = env->prev;
    delete env;
    env = old_env;
    while (p)
      {
	Entry* next = p->next_env;
	p->bucket->current_binding = p->prev;
	delete p;
	p = next;
      }
  }

  void set(const K& key, T value)
  {
    Bucket* bucket;
    Map::iterator p = map.find(key);
    if (p == map.end())
      bucket = map[key] = new Bucket(key);
    else
      bucket = p->second;
    if (bucket->current_binding->env == env)
      throw AlreadyDefined();
    bucket->current_binding = new Entry(env, bucket, bucket->current_binding, value);
  }

  T get(const K& key) const
  {
    Map::const_iterator p = map.find(key);
    if (p != map.end() && p->second->current_binding)
      return p->second->current_binding->value;
    else
      throw NotFound();
  }

  bool defined(const K& key) const
  {
    Map::const_iterator p = map.find(key);
    return p != map.end() && p->second->current_binding;
  }

  class NotFound { };
  class AlreadyDefined { };

private:
  struct Entry;
  struct Env;

  struct Bucket
  {
    Bucket(const K& k) : key(k) { }

    K key;
    Entry* current_binding;
  };

  struct Entry
  {
    Entry(Env* e, Bucket* b, Entry* p, const T& v) : env(e), bucket(b), next_env(0), prev(p), value(v) { }

    Env* env;
    Bucket* bucket;
    Entry* next_env;
    Entry* prev;
    T value;
  };

  struct Env
  {
    Env(Env* p) : prev(p) { }

    Env* prev;
    Entry* first;
  };

  typedef HASH_MAP_NS::hash_map<const K,Bucket*,HashFcn,EqualKey,Alloc> Map;

  Env* env;
  Map map;
};

#endif // __ScopedHashMap_hh__
