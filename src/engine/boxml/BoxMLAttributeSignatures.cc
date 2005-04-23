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

#include <config.h>

#include "BoxMLAttributeParsers.hh"
#include "BoxMLAttributeSignatures.hh"

DEFINE_ATTRIBUTE(BoxML, Text, "color", color, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Text, "background", background, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Text, "size", size, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Text, "width", width, true, false, false, false, "auto");
DEFINE_ATTRIBUTE(BoxML, Ink, "color", color, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Space, "width", width, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, Space, "height", height, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, Space, "depth", depth, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, H, "spacing", spacing, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, HV, "spacing", spacing, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, HV, "indent", indent, true, false, false, false, "left");
DEFINE_ATTRIBUTE(BoxML, HV, "minlinespacing", minlinespacing, true, false, false, false, "1em");
DEFINE_ATTRIBUTE(BoxML, HOV, "spacing", spacing, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, HOV, "indent", indent, true, false, false, false, "left");
DEFINE_ATTRIBUTE(BoxML, HOV, "minlinespacing", minlinespacing, true, false, false, false, "1em");
DEFINE_ATTRIBUTE(BoxML, V, "enter", enter, true, false, false, false, "1");
DEFINE_ATTRIBUTE(BoxML, V, "exit", exit, true, false, false, false, "-1");
DEFINE_ATTRIBUTE(BoxML, V, "indent", indent, true, false, false, false, "left");
DEFINE_ATTRIBUTE(BoxML, V, "minlinespacing", minlinespacing, true, false, false, false, "1em");
DEFINE_ATTRIBUTE(BoxML, Layout, "width", width, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Layout, "height", height, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Layout, "depth", depth, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, At, "x", x, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, At, "y", y, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Action, "selection", selection, true, true, false, false, "1");
DEFINE_ATTRIBUTE(BoxML, Action, "actiontype", actiontype, true, true, false, false, "toggle");
DEFINE_ATTRIBUTE(BoxML, Decor, "type", type, true, true, false, false, "box");
DEFINE_ATTRIBUTE(BoxML, Decor, "color", color, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Decor, "thickness", thickness, true, false, false, false, "1px");
