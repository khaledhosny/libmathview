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

#ifndef __Fragment_hh__
#define __Fragment_hh__

#include <list>

#include "Model.hh"
#include "Object.hh"
#include "SmartPtr.hh"
#include "Location.hh"

class Fragment : public Object
{
protected:
  Fragment(const Model::Element&, const Model::Element&, const BoundingBox&);
  virtual ~Fragment();

public:
  static SmartPtr<Fragment> create(const Model::Element&, const Model::Element&, const BoundingBox&);

  Model::Element getOldRoot(void) const { return oldRoot; }
  Model::Element getNewRoot(void) const { return newRoot; }

  void addDependency(const SmartPtr<Fragment>&);
  bool visit(std::list<SmartPtr<Fragment > >&);
  scaled getX(void) const { return x; }
  scaled getY(void) const { return y; }
  const BoundingBox& getBoundingBox(void) const { return box; }
  void setPosition(const scaled&, const scaled&);

private:
  enum FragmentColor { WHITE, GRAY, BLACK };
  FragmentColor color;

  Model::Element oldRoot;
  Model::Element newRoot;
  std::list<SmartPtr<Fragment> > dependencies;
  scaled x;
  scaled y;
  BoundingBox box;
};

#endif // __Fragment_hh__
