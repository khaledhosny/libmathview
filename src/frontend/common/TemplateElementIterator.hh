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

#ifndef __TemplateElementIterator_hh__
#define __TemplateElementIterator_hh__

template <class Model>
class TemplateElementIterator
{
public:
  TemplateElementIterator(const typename Model::Element& root, const String& ns = "*", const String& n = "*")
    : namespaceURI(ns), name(n), currentElement(typename Model::Element())
  { 
    assert(root);
    if (typename Model::Node p = Model::getFirstChild(Model::asNode(root)))
      currentElement = findValidNode(p);
  }

  typename Model::Element element(void) const { return currentElement; }
  bool more(void) const { return currentElement; }
  bool valid(const typename Model::Node& p) const
  {
    assert(p);
    return (Model::getNodeType(p) == Model::ELEMENT_NODE)
      && (namespaceURI == "*" || namespaceURI == Model::getNodeNamespaceURI(p))
      && (name == "*" || name == Model::getNodeName(p));
  }
  void next(void)
  { 
    assert(currentElement);
    currentElement = findValidNode(Model::getNextSibling(Model::asNode(currentElement)));
  }

protected:
  typename Model::Element
  findValidNode(const typename Model::Node& p0) const
  {
    for (typename Model::Node p = p0; p; p = Model::getNextSibling(p))
      if (valid(p)) return Model::asElement(p);
    return typename Model::Element();
  }
    
  String namespaceURI;
  String name;
  typename Model::Element currentElement;
};

#endif // __TemplateElementIterator_hh__
