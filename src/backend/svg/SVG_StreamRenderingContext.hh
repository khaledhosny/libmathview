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

#ifndef __SVG_StreamRenderingContext_hh__
#define __SVG_StreamRenderingContext_hh__

#include <iostream>

#include "String.hh"
#include "SVG_RenderingContext.hh"

class GMV_BackEnd_EXPORT SVG_StreamRenderingContext : public SVG_RenderingContext
{
public:
  SVG_StreamRenderingContext(const SmartPtr<class AbstractLogger>&, std::ostream&);
  virtual ~SVG_StreamRenderingContext();

  virtual void wrapperStart(const scaled&, const scaled&, const BoundingBox&,
			    const SmartPtr<class Element>&);
  virtual void wrapperEnd(void);

protected:
  virtual String getId(const SmartPtr<class Element>&) const;
  virtual void beginDocument(const BoundingBox&);
  virtual void endDocument(void);
  virtual void metadata(const String&);
  virtual void text(const scaled&, const scaled&, const String&, const scaled&,
		    const RGBColor&, const RGBColor&, const scaled&, const String&);
  virtual void rect(const scaled&, const scaled&, const scaled&, const scaled&,
		    const RGBColor&, const RGBColor&, const scaled&);
  virtual void line(const scaled&, const scaled&, const scaled&, const scaled&,
		    const RGBColor&, const scaled&);
private:
  std::ostream& output;
};

#endif // __SVG_StreamRenderingContext_hh__
