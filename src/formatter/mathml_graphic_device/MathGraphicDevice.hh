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

#ifndef __MathGraphicDevice_hh__
#define __MathGraphicDevice_hh__

#include "Area.hh"
#include "AreaFactory.hh"
#include "MathFormattingContext.hh"
#include "Length.hh"
#include "Object.hh"
#include "DOM.hh"
#include "scaled.hh"

class MathGraphicDevice : public Object
{
protected:
  MathGraphicDevice(const SmartPtr<AreaFactory>& f) : factory(f) { }
  virtual ~MathGraphicDevice() = 0;

public:
  // Length evaluation, fundamental properties

  virtual double dpi(const MathFormattingContext& context) const;
  virtual scaled evaluate(const MathFormattingContext& context,
			  const Length& length, const scaled& defaultValue) const;
  virtual scaled em(const MathFormattingContext& context) const;
  virtual scaled ex(const MathFormattingContext& context) const;
  virtual scaled axis(const MathFormattingContext& context) const;
  virtual scaled defaultLineThickness(const MathFormattingContext& context) const;

  // token formatting

  virtual AreaRef string(const MathFormattingContext& context, const DOM::GdomeString& str) const = 0;
  virtual AreaRef glyph(const MathFormattingContext& context,
			const DOM::GdomeString& alt, const DOM::GdomeString& fontFamily,
			unsigned long index) const = 0;

  // layout schemata

  virtual AreaRef fraction(const MathFormattingContext& context,
			   const AreaRef& numerator, const AreaRef& denominator,
			   const Length& lineThickness) const = 0;
  virtual AreaRef bevelledFraction(const MathFormattingContext& context,
				   const AreaRef& numerator, const AreaRef& denominator) const = 0;
  virtual AreaRef radical(const MathFormattingContext& context,
			  const AreaRef& radicand, const AreaRef& index) const = 0;
  virtual AreaRef script(const MathFormattingContext& context,
			 const AreaRef& subScript, const Length& subScriptShift,
			 const AreaRef& superScript, const Length& superScriptShift) const = 0;
  virtual AreaRef multiScripts(const MathFormattingContext& context,
			       const AreaRef& base,
			       const std::vector<AreaRef>& subScripts,
			       const std::vector<AreaRef>& preSubScripts,
			       const Length& subScriptShift,
			       const std::vector<AreaRef>& superScripts,
			       const std::vector<AreaRef>& preSuperScripts,
			       const Length& superScriptShift) const = 0;
  virtual AreaRef underOver(const MathFormattingContext& context,
			    const AreaRef& base,
			    const AreaRef& underScript, bool accentUnder,
			    const AreaRef& overScript, bool accent) const = 0;
  virtual AreaRef longDivision(const MathFormattingContext& context,
			       const AreaRef& base) const = 0;
  virtual AreaRef actuarial(const MathFormattingContext& context,
			    const AreaRef& base) const = 0;

  // factory methods

  virtual AreaRef horizontalArray(const MathFormattingContext& context,
		  		  const std::vector<AreaRef>& areas) const;
  virtual AreaRef verticalArray(const MathFormattingContext& context,
		  		const std::vector<AreaRef>& areas,
				unsigned ref) const;
  virtual AreaRef overlapArray(const MathFormattingContext& context,
		  	       const std::vector<AreaRef>& areas) const;
  virtual AreaRef wrapper(const MathFormattingContext& context, const AreaRef& area) const;
  virtual AreaRef horizontalSpace(const MathFormattingContext& context,
				  const scaled& width) const;
  virtual AreaRef verticalSpace(const MathFormattingContext& context,
				const scaled& height, const scaled& depth) const;
  virtual AreaRef horizontalFiller(const MathFormattingContext& context) const;
  virtual AreaRef verticalFiller(const MathFormattingContext& context) const;
  virtual AreaRef color(const MathFormattingContext& context, const AreaRef& area) const;
  virtual AreaRef ink(const MathFormattingContext& context, const AreaRef& area) const;
  virtual AreaRef box(const MathFormattingContext& context, const AreaRef& area, const BoundingBox& box) const;
  virtual AreaRef shift(const MathFormattingContext& context, const AreaRef& area, const scaled& s) const;

  // macro areas

  virtual AreaRef horizontalLine(const MathFormattingContext& context,
				 const scaled& thickness) const;
  virtual AreaRef verticalLine(const MathFormattingContext& context,
			       const scaled& thickness) const;
  virtual AreaRef center(const MathFormattingContext& context, const AreaRef& area) const;
  virtual AreaRef left(const MathFormattingContext& context, const AreaRef& area) const;
  virtual AreaRef right(const MathFormattingContext& context, const AreaRef& area) const;
  virtual AreaRef middle(const MathFormattingContext& context, const AreaRef& area) const;
  virtual AreaRef top(const MathFormattingContext& context, const AreaRef& area) const;
  virtual AreaRef bottom(const MathFormattingContext& context, const AreaRef& area) const;
  virtual AreaRef freeze(const MathFormattingContext& context, const AreaRef& area) const;

protected:
  SmartPtr<AreaFactory> factory;
};

#endif // __MathGraphicDevice_hh__
