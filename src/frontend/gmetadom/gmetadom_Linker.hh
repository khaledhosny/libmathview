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

#ifndef __gmetadom_Linker_hh__
#define __gmetadom_Linker_hh__

#include "gmetadom.hh"
#include "HashMap.hh"
#include "SmartPtr.hh"

class gmetadom_Linker
{
public:
  gmetadom_Linker(void);
  ~gmetadom_Linker();

  void add(const DOM::Node&, class Element*);
  bool remove(const DOM::Node&);
  bool remove(class Element*);
  class Element* assoc(const DOM::Node&) const;
  DOM::Node assoc(class Element*) const;

protected:
  struct DOM_hash
  {
    size_t operator()(const DOM::Node& node) const
    {
      assert(node);
      return reinterpret_cast<size_t>(node.id());
    }
  };

  struct Element_hash
  {
    size_t operator()(class Element* elem) const
    {
      assert(elem);
      return reinterpret_cast<size_t>(elem);
    }
  };

  typedef HASH_MAP_NS::hash_map<DOM::Node, class Element*, DOM_hash> ForwardMap;
  typedef HASH_MAP_NS::hash_map<class Element*, DOM::Node, Element_hash> BackwardMap;
  ForwardMap forwardMap;
  BackwardMap backwardMap;
};

#endif // __gmetadom_Linker_hh__
