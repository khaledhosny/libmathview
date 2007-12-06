// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#ifndef __BoxArea_hh__
#define __BoxArea_hh__

#include "BinContainerArea.hh"

class GMV_MathView_EXPORT BoxArea : public BinContainerArea
{
protected:
  BoxArea(const AreaRef&, const BoundingBox&);
  virtual ~BoxArea() { }

public:
  static SmartPtr<BoxArea> create(const AreaRef& area, const BoundingBox& b) { return new BoxArea(area, b); }
  virtual AreaRef clone(const AreaRef& area) const { return create(area, bbox); }

  virtual BoundingBox box(void) const { return bbox; }
  virtual AreaRef fit(const scaled&, const scaled&, const scaled&) const;
  virtual void strength(int&, int&, int&) const;

  virtual bool searchByCoords(class AreaId&, const scaled&, const scaled&) const;

private:
  BoundingBox bbox;
};

#endif // __BoxArea_hh__
