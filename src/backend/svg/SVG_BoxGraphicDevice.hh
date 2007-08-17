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

#ifndef __SVG_BoxGraphicDevice_hh__
#define __SVG_BoxGraphicDevice_hh__

#include "BoxGraphicDevice.hh"

class SVG_BoxGraphicDevice : public BoxGraphicDevice
{
protected:
  SVG_BoxGraphicDevice(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&);
  virtual ~SVG_BoxGraphicDevice();

public:
  static SmartPtr<SVG_BoxGraphicDevice> create(const SmartPtr<class AbstractLogger>&, 
					       const SmartPtr<class Configuration>&);

  virtual AreaRef wrapper(const class FormattingContext&, const AreaRef& area) const;
};

#endif // __SVG_BoxGraphicDevice_hh__
