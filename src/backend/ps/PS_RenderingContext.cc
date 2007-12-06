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

#include <config.h>

#include <cassert>
#include <iomanip>

#include "AbstractLogger.hh"
#include "PS_RenderingContext.hh"
#include "scaledAux.hh"
#include "BoundingBoxAux.hh"
#include "RGBColorAux.hh"
#include "TFM.hh"
#include "TFMFont.hh"


PS_RenderingContext::PS_RenderingContext(const SmartPtr<AbstractLogger>& l)
  : logger(l)
{
  assert(logger);
}

PS_RenderingContext::~PS_RenderingContext()
{ }

void
PS_RenderingContext::documentEnd()
{
  endDocument();
}

void
PS_RenderingContext::fill(const scaled& x, const scaled& y, const BoundingBox& box)
{
  rect(x, (y + box.height), 
       box.horizontalExtent(), box.verticalExtent(),
       getForegroundColor(), getForegroundColor(), scaled::zero());
}

void
PS_RenderingContext::draw(const scaled& x, const scaled& y, 
			  const SmartPtr<TFMFont>& font,
			  Char8 index) 
{
  SmartPtr<TFM> tfm = font->getTFM();
  assert(tfm);
  std::ostringstream familyS;

  familyS << tfm->getFamily() << tfm->getDesignSize().toInt();
  
  const String family = familyS.str();
  const String content(1, index);
  text(x, y, family, font->getSize(),
       getForegroundColor(), getForegroundColor(), scaled::zero(), content);
}

void
PS_RenderingContext::beginDocument(const BoundingBox&)
{ }

void
PS_RenderingContext::endDocument()
{ }

void
PS_RenderingContext::text(const scaled&, const scaled&, const String&, const scaled&,
	   	          const RGBColor&, const RGBColor&, const scaled&, const String&)
{ }

void
PS_RenderingContext::rect(const scaled&, const scaled&, const scaled&, const scaled&,
			  const RGBColor&, const RGBColor&, const scaled&)
{ }

void
PS_RenderingContext::line(const scaled&, const scaled&, const scaled&, const scaled&,
			  const RGBColor&, const scaled&)
{ }
