// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include <config.h>

#include <algorithm>

#include "Fragment.hh"

Fragment::Fragment(const Model::Element& _oldRoot, const Model::Element& _newRoot, const BoundingBox& _box)
  : color(WHITE), oldRoot(_oldRoot), newRoot(_newRoot), box(_box)
{ }

Fragment::~Fragment()
{ }

SmartPtr<Fragment>
Fragment::create(const Model::Element& _oldRoot, const Model::Element& _newRoot, const BoundingBox& _box)
{ return new Fragment(_oldRoot, _newRoot, _box); }

void
Fragment::addDependency(const SmartPtr<Fragment>& frag)
{
  if (std::find(dependencies.begin(), dependencies.end(), frag) == dependencies.end())
    dependencies.push_back(frag);
}

bool
Fragment::visit(std::list<SmartPtr<Fragment> >& sortedFragmentList)
{
  if (color == GRAY)
    return true;

  if (color == WHITE)
    {
      color = GRAY;
      for (std::list<SmartPtr<Fragment> >::iterator f = dependencies.begin(); f != dependencies.end(); f++)
	if ((*f)->visit(sortedFragmentList))
	  return true;
      color = BLACK;
      sortedFragmentList.push_back(this);
    }
  return false;
}

void
Fragment::setPosition(const scaled& ox, const scaled& oy)
{
  x = ox;
  y = oy;
}

