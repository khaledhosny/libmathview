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

#ifndef __AreaId_hh__
#define __AreaId_hh__

#include <vector>

#include "Area.hh"

class AreaId
{
public:
  AreaId(const AreaRef& r) : root(r) { };

  void append(int);
  void append(int, const AreaRef&);
  void append(int, const AreaRef&, const scaled&, const scaled&);
  void pop_back(void);

  bool empty(void) const { return path.empty(); }
  int length(void) const { return path.size(); }
  AreaRef getRoot(void) const { return root; }

  AreaRef getArea(int = -1) const;
  void getOrigin(scaled&, scaled&, int = -1) const;

  typedef std::vector<int> PathVector;
  typedef std::vector<AreaRef> AreaVector;
  typedef std::vector< std::pair<scaled,scaled> > OriginVector;

  const PathVector& getPath(void) const { return path; }
  const AreaVector& getAreas(void) const { validateAreas(); return area; }
  const OriginVector& getOrigins(void) const { validateOrigins(); return origin; }

protected:
  AreaId(const AreaRef& r, PathVector::const_iterator begin, PathVector::const_iterator end)
    : root(r), path(begin, end) { }

  void getOriginAux(int, scaled&, scaled&) const;

private:
  void validateAreas(void) const;
  void validateOrigins(void) const;

  const AreaRef root;
  PathVector path;
  mutable AreaVector area;
  mutable OriginVector origin;
};

#endif // __AreaId_hh__

