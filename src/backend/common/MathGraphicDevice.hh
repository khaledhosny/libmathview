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

#ifndef __MathGraphicDevice_hh__
#define __MathGraphicDevice_hh__

#include "String.hh"
#include "GraphicDevice.hh"

class GMV_MathView_EXPORT MathGraphicDevice : public GraphicDevice
{
protected:
  MathGraphicDevice(const SmartPtr<class AbstractLogger>&);
  virtual ~MathGraphicDevice();

public:
  virtual void clearCache(void) const;

  // Length evaluation, fundamental properties

  virtual scaled axis(const class FormattingContext&) const;
  virtual scaled ex(const class FormattingContext&) const;

  // token formatting

  AreaRef string(const class FormattingContext&, const String& str) const;
  virtual AreaRef glyph(const class FormattingContext&,
			const String& alt, const String& fontFamily,
			unsigned long index) const;

  // layout schemata

  virtual AreaRef fraction(const class FormattingContext&,
			   const AreaRef& numerator, const AreaRef& denominator,
			   const Length& lineThickness) const;
  virtual AreaRef bevelledFraction(const class FormattingContext&,
				   const AreaRef& numerator, const AreaRef& denominator,
				   const Length& lineThickness) const;
  virtual AreaRef radical(const class FormattingContext&,
			  const AreaRef& radicand, const AreaRef& index) const;
  virtual AreaRef script(const class FormattingContext&,
			 const AreaRef& base,
			 const AreaRef& subScript, const Length& subScriptShift,
			 const AreaRef& superScript, const Length& superScriptShift) const;
  virtual AreaRef multiScripts(const class FormattingContext&,
			       const AreaRef& base,
			       const std::vector<AreaRef>& subScripts,
			       const std::vector<AreaRef>& preSubScripts,
			       const Length& subScriptShift,
			       const std::vector<AreaRef>& superScripts,
			       const std::vector<AreaRef>& preSuperScripts,
			       const Length& superScriptShift) const;
  virtual AreaRef underOver(const class FormattingContext&,
			    const AreaRef& base,
			    const AreaRef& underScript, bool accentUnder,
			    const AreaRef& overScript, bool accent) const;
  virtual AreaRef enclose(const class FormattingContext&,
			  const AreaRef& base,
			  const String& notation) const;

  // extra methods

  virtual AreaRef wrapper(const class FormattingContext&, const AreaRef& area) const;
  virtual AreaRef dummy(const class FormattingContext& context) const;

protected:
  AreaRef stretchedString(const class FormattingContext&, const String& str) const;
  AreaRef unstretchedString(const class FormattingContext&, const String& str) const;
  AreaRef stretchStringV(const class FormattingContext&,
			 const String& str,
			 const scaled& height,
			 const scaled& depth) const;
  virtual void calculateDefaultScriptShift(const class FormattingContext&,
					   const BoundingBox& baseBox,
					   const BoundingBox& subScriptBox,
					   const BoundingBox& superScriptBox,
					   scaled& v, scaled& u) const;
  virtual void calculateScriptShift(const class FormattingContext&,
				    const BoundingBox& baseBox,
				    const BoundingBox& subScriptBox,
				    const Length& subScriptMinShift,
				    const BoundingBox& superScriptBox,
				    const Length& superScriptMinShift,
				    scaled& v, scaled& u) const;
};

#endif // __MathGraphicDevice_hh__
