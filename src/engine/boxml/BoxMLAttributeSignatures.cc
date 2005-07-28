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

#include <cassert>

#include "BoxMLAttributeParsers.hh"
#include "BoxMLAttributeSignatures.hh"

DEFINE_ATTRIBUTE(BoxML, Text, color, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Text, background, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Text, size, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Text, width, true, false, false, false, "auto");
DEFINE_ATTRIBUTE(BoxML, Ink, color, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Space, width, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, Space, height, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, Space, depth, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, H, spacing, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, HV, spacing, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, HV, indent, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, HV, minlinespacing, true, false, false, false, "1.8em");
DEFINE_ATTRIBUTE(BoxML, HOV, spacing, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, HOV, indent, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, HOV, minlinespacing, true, false, false, false, "1.8em");
DEFINE_ATTRIBUTE(BoxML, V, enter, true, false, false, false, "1");
DEFINE_ATTRIBUTE(BoxML, V, exit, true, false, false, false, "-1");
DEFINE_ATTRIBUTE(BoxML, V, indent, true, false, false, false, "0pt");
DEFINE_ATTRIBUTE(BoxML, V, minlinespacing, true, false, false, false, "1.8em");
DEFINE_ATTRIBUTE(BoxML, Layout, width, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Layout, height, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Layout, depth, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, At, x, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, At, y, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Action, selection, true, true, false, false, "1");
DEFINE_ATTRIBUTE(BoxML, Action, actiontype, true, true, false, false, "toggle");
DEFINE_ATTRIBUTE(BoxML, Decor, type, true, true, false, false, "box");
DEFINE_ATTRIBUTE(BoxML, Decor, color, true, false, false, false, 0);
DEFINE_ATTRIBUTE(BoxML, Decor, thickness, true, false, false, false, "1px");
