// Copyright (C) 2000-2001, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://www.cs.unibo.it/helm/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef gmetadom_hh
#define gmetadom_hh

#include <GdomeSmartDOM.hh>

#include "String.hh"

namespace DOM = GdomeSmartDOM;

namespace GdomeSmartDOMExt {

  bool nodeIsBlank(const DOM::Node&);
  DOM::GdomeString elementValue(const DOM::Element&);
  DOM::GdomeString nodeLocalName(const DOM::Node&);

  inline String fromDOMString(const DOM::GdomeString& s) { return s; }
  inline DOM::GdomeString toDOMString(const String& s) { return s; }

}

namespace DOMX = GdomeSmartDOMExt;

#endif // gmetadom_hh
