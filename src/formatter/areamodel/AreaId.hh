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

  void append(AreaIndex);
  void append(AreaIndex, const AreaRef&);
  void append(AreaIndex, const AreaRef&, const scaled&, const scaled&);
  void pop_back(void);

  bool empty(void) const { return pathV.empty(); }
  int size(void) const { return pathV.size(); }

  AreaRef getArea(int = -1) const;
  void getOrigin(scaled&, scaled&) const;
  void getOrigin(int, scaled&, scaled&) const;
  void getOrigin(int, int, scaled&, scaled&) const;
  void accumulateOrigin(scaled&, scaled&) const;
  void accumulateOrigin(int, scaled&, scaled&) const;
  void accumulateOrigin(int, int, scaled&, scaled&) const;
  CharIndex getLength(int = 0, int = -1) const;

  typedef std::vector<AreaIndex> PathVector;
  typedef std::vector<AreaRef> AreaVector;
  typedef std::vector< std::pair<scaled,scaled> > OriginVector;
  typedef std::vector<CharIndex> LengthVector;

  const PathVector& getPath(void) const { return pathV; }
  const AreaVector& getAreas(void) const { validateAreas(); return areaV; }
  const OriginVector& getOrigins(void) const { validateOrigins(); return originV; }
  const LengthVector& getLengths(void) const { validateLengths(); return lengthV; }

protected:
  AreaId(const AreaRef& r, const PathVector::const_iterator& begin, const PathVector::const_iterator& end)
    : root(r), pathV(begin, end) { }

  void accumulateOriginAux(const OriginVector::const_iterator&, const OriginVector::const_iterator&,
			   scaled&, scaled&) const;
  void accumulateLengthAux(const LengthVector::const_iterator&, const LengthVector::const_iterator&, CharIndex&) const;

private:
  void validateAreas(void) const;
  void validateOrigins(void) const;
  void validateLengths(void) const;

  const AreaRef root;
  PathVector pathV;
  mutable AreaVector areaV;
  mutable OriginVector originV;
  mutable LengthVector lengthV;
};

#endif // __AreaId_hh__

