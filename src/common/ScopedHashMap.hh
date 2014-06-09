// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#ifndef __ScopedHashMap_hh__
#define __ScopedHashMap_hh__

#include <unordered_map>

template <class K, class T, class HashFcn = std::tr1::hash<K>, class EqualKey = std::equal_to<K>, class Alloc = std::allocator<T> >
class ScopedHashMap
{
public:
  ScopedHashMap(void) : env(0)
  { push(); }

  ~ScopedHashMap()
  {
    while (env) pop();
    for (typename Map::iterator p = map.begin(); p != map.end(); p++)
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
    typename Map::iterator p = map.find(key);
    if (p == map.end())
      bucket = map[key] = new Bucket(key);
    else
      bucket = p->second;

    if (bucket->current_binding && bucket->current_binding->env == env)
      bucket->current_binding->value = value;
    else
      {
        bucket->current_binding = new Entry(env, bucket, bucket->current_binding, env->first, value);
	env->first = bucket->current_binding;
      }
  }

  T get(const K& key) const
  {
    typename Map::const_iterator p = map.find(key);
    if (p != map.end() && p->second->current_binding)
      return p->second->current_binding->value;
    else
      throw NotFound();
  }

  bool defined(const K& key) const
  {
    typename Map::const_iterator p = map.find(key);
    return p != map.end() && p->second->current_binding;
  }

  bool definedInScope(const K& key) const
  {
    typename Map::const_iterator p = map.find(key);
    return p != map.end() && p->second->current_binding &&
      p->second->current_binding->env == env;
  }

  class NotFound { };

private:
  struct Entry;
  struct Env;

  struct Bucket
  {
    Bucket(const K& k) : key(k), current_binding(0) { }

    K key;
    Entry* current_binding;
  };

  struct Entry
  {
    Entry(Env* e, Bucket* b, Entry* p, Entry* p_env, const T& v) : env(e), bucket(b), next_env(p_env), prev(p), value(v) { }

    Env* env;
    Bucket* bucket;
    Entry* next_env;
    Entry* prev;
    T value;
  };

  struct Env
  {
    Env(Env* p) : prev(p), first(0) { }

    Env* prev;
    Entry* first;
  };

  typedef std::unordered_map<const K,Bucket*,HashFcn,EqualKey,Alloc> Map;

  Env* env;
  Map map;
};

#endif // __ScopedHashMap_hh__
