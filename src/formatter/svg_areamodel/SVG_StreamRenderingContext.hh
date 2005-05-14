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

#ifndef __SVG_StreamRenderingContext_hh__
#define __SVG_StreamRenderingContext_hh__

#include <iostream>

#include "String.hh"
#include "SVG_RenderingContext.hh"

class SVG_StreamRenderingContext : public SVG_RenderingContext
{
public:
  SVG_StreamRenderingContext(const SmartPtr<class AbstractLogger>&, std::ostream&);
  virtual ~SVG_StreamRenderingContext();

  void prologue(const scaled&, const scaled&) const;
  void epilogue(void) const;
  virtual void fill(const scaled&, const scaled&, const BoundingBox&) const;
  virtual void draw(const scaled&, const scaled&, const SmartPtr<class TFM_T1Font>&, Char8) const;

  static String scaledToSVG(const scaled&);
  static String RGBColorToSVG(const RGBColor&);

private:
  std::ostream& output;
};

#endif // __SVG_StreamRenderingContext_hh__
