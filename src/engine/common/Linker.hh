// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#ifndef __Linker_hh__
#define __Linker_hh__

#include "DOM.hh"
#include "HashMap.hh"
#include "SmartPtr.hh"
#include "WeakPtr.hh"
#include "Object.hh"

class Linker : public Object
{
protected:
  Linker(void);
  virtual ~Linker();

public:
  static SmartPtr<Linker> create(void) 
  { return new Linker(); }

  SmartPtr<class Element> get(const DOM::Element&) const;
  void add(const DOM::Element&, const SmartPtr<class Element>&);
  bool remove(const DOM::Element&);
  
protected:
  struct DOM_hash : public std::unary_function<DOM::Node,size_t>
  {
    size_t operator()(const DOM::Node& node) const
    {
      assert(node);
      return reinterpret_cast<size_t>(node.id());
    }
  };

  typedef HASH_MAP_NS::hash_map<DOM::Node,WeakPtr<class Element>,DOM_hash> DOMNodeMap;
  mutable DOMNodeMap nodeMap;
};

#endif // __Linker_hh__
