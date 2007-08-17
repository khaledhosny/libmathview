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

#ifndef __SVG_EvalRenderingContext_hh__
#define __SVG_EvalRenderingContext_hh__

#include "BoundingBox.hh"
#include "StringHash.hh"
#include "HashMap.hh"
#include "MathView.hh"
#include "SVG_RenderingContext.hh"

class SVG_EvalRenderingContext : public SVG_RenderingContext
{
public:
  SVG_EvalRenderingContext(const SmartPtr<class AbstractLogger>&,
			   const SmartPtr<MathView>&);
  virtual ~SVG_EvalRenderingContext();

  virtual void wrapperStart(const scaled&, const scaled&, const BoundingBox&,
			    const SmartPtr<Element>&);
  virtual void wrapperEnd(void);

  SmartPtr<class Location> getLocation(const String&) const;

private:
  SmartPtr<MathView> view;
  typedef HASH_MAP_NS::hash_map<String,SmartPtr<class Location>,StringHash,StringEq> IdMap;
  IdMap idMap;
};

#endif // __SVG_EvalRenderingContext_hh__
