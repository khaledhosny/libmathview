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

#ifndef __BoxMLAttributeSignatures_hh__
#define __BoxMLAttributeSignatures_hh__

#include "AttributeSignature.hh"

DECLARE_ATTRIBUTE(BoxML, Text, color);
DECLARE_ATTRIBUTE(BoxML, Text, background);
DECLARE_ATTRIBUTE(BoxML, Text, size);
DECLARE_ATTRIBUTE(BoxML, Text, width);
DECLARE_ATTRIBUTE(BoxML, Ink, color);
DECLARE_ATTRIBUTE(BoxML, Space, width);
DECLARE_ATTRIBUTE(BoxML, Space, height);
DECLARE_ATTRIBUTE(BoxML, Space, depth);
DECLARE_ATTRIBUTE(BoxML, H, spacing);
DECLARE_ATTRIBUTE(BoxML, HV, spacing);
DECLARE_ATTRIBUTE(BoxML, HV, indent);
DECLARE_ATTRIBUTE(BoxML, HV, minlinespacing);
DECLARE_ATTRIBUTE(BoxML, HOV, spacing);
DECLARE_ATTRIBUTE(BoxML, HOV, indent);
DECLARE_ATTRIBUTE(BoxML, HOV, minlinespacing);
DECLARE_ATTRIBUTE(BoxML, V, enter);
DECLARE_ATTRIBUTE(BoxML, V, exit);
DECLARE_ATTRIBUTE(BoxML, V, indent);
DECLARE_ATTRIBUTE(BoxML, V, minlinespacing);
DECLARE_ATTRIBUTE(BoxML, Layout, width);
DECLARE_ATTRIBUTE(BoxML, Layout, height);
DECLARE_ATTRIBUTE(BoxML, Layout, depth);
DECLARE_ATTRIBUTE(BoxML, At, x);
DECLARE_ATTRIBUTE(BoxML, At, y);
DECLARE_ATTRIBUTE(BoxML, Action, selection);
DECLARE_ATTRIBUTE(BoxML, Action, actiontype);
DECLARE_ATTRIBUTE(BoxML, Decor, type);
DECLARE_ATTRIBUTE(BoxML, Decor, color);
DECLARE_ATTRIBUTE(BoxML, Decor, thickness);

#endif // __BoxMLAttributeSignatures_hh__
