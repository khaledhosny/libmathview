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

#ifndef __SVG_TFMComputerModernMathGraphicDevice_hh__
#define __SVG_TFMComputerModernMathGraphicDevice_hh__

#include "TFMComputerModernMathGraphicDevice.hh"

class SVG_TFMComputerModernMathGraphicDevice : public TFMComputerModernMathGraphicDevice
{
protected:
  SVG_TFMComputerModernMathGraphicDevice(const SmartPtr<class AbstractLogger>&,
					 const SmartPtr<class Configuration>&);
  virtual ~SVG_TFMComputerModernMathGraphicDevice();

public:
  static SmartPtr<SVG_TFMComputerModernMathGraphicDevice> create(const SmartPtr<class AbstractLogger>&,
								 const SmartPtr<class Configuration>&);

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
  virtual AreaRef wrapper(const class FormattingContext&, const AreaRef& area) const;
};

#endif // __SVG_TFMComputerModernMathGraphicDevice_hh__
