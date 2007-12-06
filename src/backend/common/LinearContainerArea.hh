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

#ifndef __LinearContainerArea_hh__
#define __LinearContainerArea_hh__

#include <vector>

#include "ContainerArea.hh"

class GMV_MathView_EXPORT LinearContainerArea : public ContainerArea
{
protected:
  LinearContainerArea(const std::vector<AreaRef>& c) : content(c) { }
  virtual ~LinearContainerArea() { }

public:
  virtual AreaRef clone(const std::vector<AreaRef>&) const = 0;

  virtual void render(class RenderingContext&, const scaled&, const scaled&) const;
  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;
  virtual AreaIndex size(void) const { return content.size(); }
  virtual AreaRef node(AreaIndex) const;
  virtual CharIndex lengthTo(AreaIndex) const;
  //virtual AreaRef replace(const class ReplacementContext&) const;

  virtual bool searchByArea(class AreaId&, const AreaRef&) const;
  virtual bool searchByIndex(class AreaId&, CharIndex) const;

  virtual SmartPtr<const class GlyphStringArea> getGlyphStringArea(void) const;
  virtual SmartPtr<const class GlyphArea> getGlyphArea(void) const;

  const std::vector<AreaRef> getChildren(void) const { return content; }

protected:
  std::vector<AreaRef> content;
};

#endif // __LinearContainerArea_hh__
