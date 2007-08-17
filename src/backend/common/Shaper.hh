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

#ifndef __Shaper_hh__
#define __Shaper_hh__

#include "Area.hh"

class GMV_MathView_EXPORT Shaper : public Object
{
protected:
  Shaper(void) { }
  virtual ~Shaper() { }

public:
  virtual void registerShaper(const SmartPtr<class ShaperManager>&, unsigned) = 0;
  virtual void unregisterShaper(const SmartPtr<class ShaperManager>&, unsigned) = 0;
  virtual void shape(class ShapingContext&) const = 0;
  virtual bool isDefaultShaper(void) const;

  virtual bool shapeCombiningChar(const ShapingContext&) const;
  virtual bool computeCombiningCharOffsetsAbove(const AreaRef&, const AreaRef&,
					        scaled&, scaled&) const;
  virtual bool computeCombiningCharOffsetsBelow(const AreaRef&, const AreaRef&,
					        scaled&) const;

protected:
  virtual AreaRef composeStretchyCharV(const SmartPtr<class AreaFactory>&,
				       const AreaRef&, const AreaRef&, const AreaRef&, const AreaRef&, const AreaRef&,
				       const scaled&) const;
  virtual AreaRef composeStretchyCharH(const SmartPtr<class AreaFactory>&,
				       const AreaRef&, const AreaRef&, const AreaRef&, const AreaRef&,
				       const scaled&) const;
};

#endif // __Shaper_hh__

