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

#ifndef __MathMLViewContext_hh__
#define __MathMLViewContext_hh__

#include "Object.hh"
#include "SmartPtr.hh"

class MathMLViewContext : public Object
{
protected:
  MathMLViewContext(const SmartPtr<class MathMLDOMLinker>&,
		    const SmartPtr<class MathMLFormattingEngineFactory>&,
		    const SmartPtr<class AreaFactory>&,
		    const SmartPtr<class ShaperManager>&);
  virtual ~MathMLViewContext();

public:
  static SmartPtr<MathMLViewContext> create(const SmartPtr<class MathMLDOMLinker>& l,
					    const SmartPtr<class MathMLFormattingEngineFactory>& ef,
					    const SmartPtr<class AreaFactory>& af,
					    const SmartPtr<class ShaperManager>& sm)
  { return new MathMLViewContext(l, ef, af, sm); }
  
  SmartPtr<class MathMLDOMLinker> linker;
  SmartPtr<class AreaFactory> areaFactory;
  SmartPtr<class ShaperManager> shaperManager;
  SmartPtr<class MathMLFormattingEngineFactory> engineFactory;
};

#endif // __MathMLViewContext_hh__
