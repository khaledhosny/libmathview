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

#ifndef __MathMLDOMLinker_hh__
#define __MathMLDOMLinker_hh__

#include "DOM.hh"
#include "Object.hh"
#include "HashMap.hh"
#include "SmartPtr.hh"
#include "WeakPtr.hh"

class MathMLDOMLinker : public Object
{
protected:
  MathMLDOMLinker(void);
  virtual ~MathMLDOMLinker();

public:
  static SmartPtr<MathMLDOMLinker> create(void)
  { return new MathMLDOMLinker(); }

  SmartPtr<class MathMLElement> get(const DOM::Element&) const;
  void add(const DOM::Element&, const SmartPtr<class MathMLElement>&);
  bool remove(const DOM::Element&);
  
protected:
  struct DOM_hash : public std::unary_function<DOM::Node,size_t>
  {
    size_t operator()(const DOM::Node& node) const
    {
      assert(node);
      return reinterpret_cast<size_t>(static_cast<GdomeNode*>(node));
    }
  };

  typedef HASH_MAP_NS::hash_map<DOM::Node,WeakPtr<class MathMLElement>,DOM_hash> DOMNodeMap;
  mutable DOMNodeMap nodeMap;
};

#endif // __MathMLDOMLinker_hh__
