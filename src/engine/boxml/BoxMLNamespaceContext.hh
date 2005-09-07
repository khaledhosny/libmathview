// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#ifndef __BoxMLNamespaceContext_hh__
#define __BoxMLNamespaceContext_hh__

#include "gmv_defines.h"
#include "NamespaceContext.hh"

class GMV_EXPORT BoxMLNamespaceContext : public NamespaceContext
{
protected:
  BoxMLNamespaceContext(const SmartPtr<class View>&,
			const SmartPtr<class BoxGraphicDevice>&);
  virtual ~BoxMLNamespaceContext();

public:
  static SmartPtr<BoxMLNamespaceContext> create(const SmartPtr<class View>& view,
						const SmartPtr<class BoxGraphicDevice>& device)
  { return new BoxMLNamespaceContext(view, device); }

  SmartPtr<class BoxGraphicDevice> getGraphicDevice(void) const;

private:
  SmartPtr<class BoxGraphicDevice> device;
};

#endif // __BoxMLNamespaceContext_hh__
