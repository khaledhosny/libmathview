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

#ifndef __FastScopedHashMap_hh__
#define __FastScopedHashMap_hh__

#include "gmv_defines.h"

template <int MAX, class T>
class GMV_MathView_EXPORT FastScopedHashMap
{
public:
  FastScopedHashMap(void) : env(0)
  { 
    push();
    for (int i = 0; i < MAX; i++)
      map[i].key = i;
  }

  ~FastScopedHashMap()
  {
    while (env) pop();
    for (int i = 0; i < MAX; i++)
      delete map[i].current_binding;
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

  void set(int key, T value)
  {
    assert(key >= 0 && key < MAX);
    Bucket* bucket = &map[key];

    if (bucket->current_binding && bucket->current_binding->env == env)
      bucket->current_binding->value = value;
    else
      {
        bucket->current_binding = new Entry(env, bucket, bucket->current_binding, env->first, value);
	env->first = bucket->current_binding;
      }
  }

  T get(int key) const
  {
    assert(key >= 0 && key < MAX);
    const Bucket* bucket = &map[key];

    if (bucket->current_binding)
      return bucket->current_binding->value;
    else
      throw NotFound();
  }

  bool defined(int key) const
  {
    assert(key >= 0 && key < MAX);
    return map[key].current_binding;
  }

  bool definedInScope(int key) const
  {
    assert(key >= 0 && key < MAX);
    return map[key].current_binding && map[key].current_binding->env == env;
  }

  class NotFound { };

private:
  struct Entry;
  struct Env;

  struct Bucket
  {
    Bucket(void) : key(-1), current_binding(0) { }

    int key;
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

  Env* env;
  Bucket map[MAX];
};

#endif // __FastScopedHashMap_hh__
