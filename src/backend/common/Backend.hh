// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

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
