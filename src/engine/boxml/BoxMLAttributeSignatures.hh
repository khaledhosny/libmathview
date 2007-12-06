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
