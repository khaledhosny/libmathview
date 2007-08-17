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

#ifndef __TemplateNodeIterator_hh__
#define __TemplateNodeIterator_hh__

#include "gmv_defines.h"

template <class Model>
class GMV_MathView_EXPORT TemplateNodeIterator
{
public:
  TemplateNodeIterator(const typename Model::Node& root) : currentNode(Model::getFirstChild(root)) { }

  typename Model::Node node(void) const { return currentNode; }
  bool more(void) const { return currentNode; }
  void next(void)
  {
    assert(currentNode);
    currentNode = Model::getNextSibling(currentNode);
  }
  
protected:
  typename Model::Node currentNode;
};

#endif // __TemplateNodeIterator_hh__
