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

#ifndef __Backend_hh__
#define __Backend_hh__

#include "Object.hh"
#include "SmartPtr.hh"

class GMV_MathView_EXPORT Backend : public Object
{
protected:
  Backend(const SmartPtr<class AbstractLogger>&,
	  const SmartPtr<class Configuration>&);
  virtual ~Backend();

public:
  SmartPtr<class ShaperManager> getShaperManager(void) const;
  void setMathGraphicDevice(const SmartPtr<class MathGraphicDevice>&);
  virtual SmartPtr<class MathGraphicDevice> getMathGraphicDevice(void) const;
#if GMV_ENABLE_BOXML
  void setBoxGraphicDevice(const SmartPtr<class BoxGraphicDevice>&);
  virtual SmartPtr<class BoxGraphicDevice> getBoxGraphicDevice(void) const;
#endif // GMV_ENABLE_BOXML

private:
  SmartPtr<class ShaperManager> shaperManager;
  SmartPtr<class MathGraphicDevice> mathGraphicDevice;
#if GMV_ENABLE_BOXML
  SmartPtr<class BoxGraphicDevice> boxGraphicDevice;
#endif // GMV_ENABLE_BOXML
};

#endif // __Backend_hh__
