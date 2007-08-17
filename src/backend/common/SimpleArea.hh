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

#ifndef __SimpleArea_hh__
#define __SimpleArea_hh__

#include "Area.hh"

class GMV_MathView_EXPORT SimpleArea : public Area
{
protected:
  SimpleArea(void) { }
  virtual ~SimpleArea() { }

public:
  virtual AreaRef fit(const scaled&, const scaled&, const scaled&) const;
  //virtual AreaRef replace(const class ReplacementContext&) const;

  virtual void      render(class RenderingContext&, const scaled&, const scaled&) const;
  virtual void      strength(int&, int&, int&) const;
  virtual AreaRef   node(AreaIndex) const;
  virtual void      origin(AreaIndex, class Point&) const;
  virtual CharIndex lengthTo(AreaIndex) const;

  virtual bool searchByArea(class AreaId&, const AreaRef&) const;
  virtual bool searchByCoords(class AreaId&, const scaled&, const scaled&) const;
  virtual bool searchByIndex(class AreaId&, CharIndex) const;
};

#endif // __SimpleArea_hh__
