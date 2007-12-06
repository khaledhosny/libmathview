// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

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
