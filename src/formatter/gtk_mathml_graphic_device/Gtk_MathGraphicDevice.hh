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

#ifndef __Gtk_MathGraphicDevice_hh__
#define __Gtk_MathGraphicDevice_hh__

#include <gtk/gtk.h>

#include "MathGraphicDevice.hh"

class Gtk_MathGraphicDevice : public MathGraphicDevice
{
protected:
  Gtk_MathGraphicDevice(GtkWidget*);
  virtual ~Gtk_MathGraphicDevice();

public:
  static SmartPtr<Gtk_MathGraphicDevice> create(GtkWidget* widget)
  { return new Gtk_MathGraphicDevice(widget); }

  virtual SmartPtr<class AreaFactory> getFactory(void) const;

  virtual AreaRef string(const MathFormattingContext& context, const String& str) const;
  virtual AreaRef glyph(const MathFormattingContext& context,
			const String& alt, const String& fontFamily,
			unsigned long index) const;

  virtual AreaRef fraction(const MathFormattingContext& context,
			   const AreaRef& numerator, const AreaRef& denominator,
			   const Length& lineThickness) const;
  virtual AreaRef bevelledFraction(const MathFormattingContext& context,
				   const AreaRef& numerator, const AreaRef& denominator,
				   const Length& lineThickness) const;
  virtual AreaRef radical(const MathFormattingContext& context,
			  const AreaRef& radicand, const AreaRef& index) const;
  virtual AreaRef script(const MathFormattingContext& context,
			 const AreaRef& base,
			 const AreaRef& subScript, const Length& subScriptShift,
			 const AreaRef& superScript, const Length& superScriptShift) const;
  virtual AreaRef multiScripts(const MathFormattingContext& context,
			       const AreaRef& base,
			       const std::vector<AreaRef>& subScripts,
			       const std::vector<AreaRef>& preSubScripts,
			       const Length& subScriptShift,
			       const std::vector<AreaRef>& superScripts,
			       const std::vector<AreaRef>& preSuperScripts,
			       const Length& superScriptShift) const;
  virtual AreaRef underOver(const MathFormattingContext& context,
			    const AreaRef& base,
			    const AreaRef& underScript, bool accentUnder,
			    const AreaRef& overScript, bool accent) const;
  virtual AreaRef longDivision(const MathFormattingContext& context,
			       const AreaRef& base) const;
  virtual AreaRef actuarial(const MathFormattingContext& context,
			    const AreaRef& base) const;
  virtual AreaRef enclose(const MathFormattingContext& context,
			  const AreaRef& base,
			  const String& notation) const;

protected:
  virtual AreaRef stretchStringV(const MathFormattingContext& context,
				 const String& str,
				 const scaled& height,
				 const scaled& depth) const;
  virtual void calculateDefaultScriptShift(const MathFormattingContext& context,
					   const BoundingBox& baseBox,
					   const BoundingBox& subScriptBox,
					   const BoundingBox& superScriptBox,
					   scaled& v, scaled& u) const;
  virtual void calculateScriptShift(const MathFormattingContext& context,
				    const BoundingBox& baseBox,
				    const BoundingBox& subScriptBox,
				    const Length& subScriptMinShift,
				    const BoundingBox& superScriptBox,
				    const Length& superScriptMinShift,
				    scaled& v, scaled& u) const;

private:
  SmartPtr<class Gtk_AreaFactory> factory;
};

#endif // __Gtk_MathGraphicDevice_hh__
