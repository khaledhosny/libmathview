// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// Copyright (C) 2013, Khaled Hosny <khaledhosny@eglug.org>.
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

#ifndef __MathShaper_hh__
#define __MathShaper_hh__

#include <hb.h>

#include "Shaper.hh"
#include "String.hh"
#include "MathFont.hh"

class MathShaper : public Shaper
{
protected:
  MathShaper(const hb_font_t* font);
  virtual ~MathShaper();

public:
  virtual void registerShaper(const SmartPtr<class ShaperManager>&, unsigned) { }
  virtual void unregisterShaper(const SmartPtr<class ShaperManager>&, unsigned) { }
  virtual bool isDefaultShaper(void) const { return false; }

  virtual void shape(class ShapingContext&) const;
  virtual bool shapeCombiningChar(const ShapingContext&) const;
  virtual bool computeCombiningCharOffsetsAbove(const AreaRef&, const AreaRef&,
                                                scaled&, scaled&) const;
  virtual bool computeCombiningCharOffsetsBelow(const AreaRef&, const AreaRef&,
                                                scaled&) const;

protected:
  virtual AreaRef getGlyphArea(unsigned, const scaled&) const = 0;

private:
  SmartPtr<class MathFont> m_mathfont;
  const hb_font_t* m_font;
};

#endif // __MathShaper_hh__

