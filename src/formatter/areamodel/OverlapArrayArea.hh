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

#ifndef __OverlapArrayArea_hh__
#define __OverlapArrayArea_hh__

#include "LinearContainerArea.hh"

class OverlapArrayArea : public LinearContainerArea
{
protected:
  OverlapArrayArea(const std::vector<AreaRef>& children) : LinearContainerArea(children) { }
  virtual ~OverlapArrayArea() { }

public:
  static SmartPtr<OverlapArrayArea> create(const std::vector<AreaRef>& children) { return new OverlapArrayArea(children); }
  virtual AreaRef clone(const std::vector<AreaRef>&) const;
  virtual AreaRef flatten(void) const;

  virtual void strength(int&, int&, int&) const;
  virtual AreaRef fit(const scaled&, const scaled&, const scaled&) const;
  virtual BoundingBox box(void) const;
  virtual void origin(unsigned, scaled&, scaled&) const;

  virtual bool searchByCoords(class AreaId&, const scaled&, const scaled&) const;

private:
  static void flattenAux(std::vector<AreaRef>&, const std::vector<AreaRef>&);  
};

#endif // __OverlapArrayArea_hh__
