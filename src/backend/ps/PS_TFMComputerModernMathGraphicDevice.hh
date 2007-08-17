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

#ifndef __PS_TFMComputerModernMathGraphicDevice_hh__
#define __PS_TFMComputerModernMathGraphicDevice_hh__

#include "TFMComputerModernMathGraphicDevice.hh"

class PS_TFMComputerModernMathGraphicDevice : public TFMComputerModernMathGraphicDevice
{
protected:
  PS_TFMComputerModernMathGraphicDevice(const SmartPtr<class AbstractLogger>&,
					 const SmartPtr<class Configuration>&);
  virtual ~PS_TFMComputerModernMathGraphicDevice();

public:
  static SmartPtr<PS_TFMComputerModernMathGraphicDevice> create(const SmartPtr<class AbstractLogger>&,
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
 };

#endif // __PS_TFMComputerModernMathGraphicDevice_hh__
