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

#include "MathMLAttributeParsers.hh"
#include "MathMLAttributeSignatures.hh"

DEFINE_ATTRIBUTE(MathML, Action, "actiontype", actiontype, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Action, "selection", selection, true, true, false, false, "1");
DEFINE_ATTRIBUTE(MathML, AlignGroup, "groupalign", groupalign, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, AlignMark, "edge", edge, true, true, false, false, "left");
DEFINE_ATTRIBUTE(MathML, Element, "class", class, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Element, "id", id, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Element, "other", other, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Enclose, "notation", notation, true, true, false, false, "longdiv");
DEFINE_ATTRIBUTE(MathML, Fenced, "close", close, true, true, false, false, ")");
DEFINE_ATTRIBUTE(MathML, Fenced, "open", open, true, true, false, false, "(");
DEFINE_ATTRIBUTE(MathML, Fenced, "separators", separators, true, true, false, true, ",");
DEFINE_ATTRIBUTE(MathML, Fraction, "bevelled", bevelled, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Fraction, "denomalign", denomalign, true, true, false, false, "center");
DEFINE_ATTRIBUTE(MathML, Fraction, "linethickness", linethickness, true, true, false, false, "1");
DEFINE_ATTRIBUTE(MathML, Fraction, "numalign", numalign, true, true, false, false, "center");
DEFINE_ATTRIBUTE(MathML, Operator, "accent", accent, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Operator, "fence", fence, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Operator, "form", form, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Operator, "largeop", largeop, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Operator, "lspace", lspace, true, true, false, false, "thickmathspace");
DEFINE_ATTRIBUTE(MathML, Operator, "maxsize", maxsize, true, true, false, false, "infinity");
DEFINE_ATTRIBUTE(MathML, Operator, "minsize", minsize, true, true, false, false, "1");
DEFINE_ATTRIBUTE(MathML, Operator, "movablelimits", movablelimits, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Operator, "rspace", rspace, true, true, false, false, "thickmathspace");
DEFINE_ATTRIBUTE(MathML, Operator, "separator", separator, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Operator, "stretchy", stretchy, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Operator, "symmetric", symmetric, true, true, false, false, "true");
DEFINE_ATTRIBUTE(MathML, Padded, "depth", depth, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Padded, "height", height, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Padded, "lspace", lspace, true, false, false, false, "0em");
DEFINE_ATTRIBUTE(MathML, Padded, "width", width, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, MultiScripts, "subscriptshift", subscriptshift, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, MultiScripts, "superscriptshift", superscriptshift, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Script, "subscriptshift", subscriptshift, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Script, "superscriptshift", superscriptshift, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Space, "depth", depth, true, true, false, false, "0ex");
DEFINE_ATTRIBUTE(MathML, Space, "height", height, true, true, false, false, "0ex");
DEFINE_ATTRIBUTE(MathML, Space, "linebreak", linebreak, true, true, false, false, "auto");
DEFINE_ATTRIBUTE(MathML, Space, "width", width, true, true, false, false, "0em");
DEFINE_ATTRIBUTE(MathML, StringLit, "lquote", lquote, true, true, false, false, "\"");
DEFINE_ATTRIBUTE(MathML, StringLit, "rquote", rquote, true, true, false, false, "\"");
DEFINE_ATTRIBUTE(MathML, Style, "color", color, true, false, true, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "background", background, true, false, true, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "mathcolor", mathcolor, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "mathbackground", mathbackground, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "displaystyle", displaystyle, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "mathsize", mathsize, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "mediummathspace", mediummathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "negativemediummathspace", negativemediummathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "negativethickmathspace", negativethickmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "negativethinmathspace", negativethinmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "negativeverythickmathspace", negativeverythickmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "negativeverythinmathspace", negativeverythinmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "negativeveryverythickmathspace", negativeveryverythickmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "negativeveryverythinmathspace", negativeveryverythinmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "scriptlevel", scriptlevel, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "scriptminsize", scriptminsize, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "scriptsizemultiplier", scriptsizemultiplier, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "thickmathspace", thickmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "thinmathspace", thinmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "verythickmathspace", verythickmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "verythinmathspace", verythinmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "veryverythickmathspace", veryverythickmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, "veryverythinmathspace", veryverythinmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Table, "align", align, true, true, false, false, "axis");
DEFINE_ATTRIBUTE(MathML, Table, "alignmentscope", alignmentscope, true, true, false, false, "true");
DEFINE_ATTRIBUTE(MathML, Table, "columnalign", columnalign, true, false, false, false, "center");
DEFINE_ATTRIBUTE(MathML, Table, "columnlines", columnlines, true, true, false, false, "none");
DEFINE_ATTRIBUTE(MathML, Table, "columnspacing", columnspacing, true, true, false, false, "0.8em");
DEFINE_ATTRIBUTE(MathML, Table, "columnwidth", columnwidth, true, true, false, false, "auto");
DEFINE_ATTRIBUTE(MathML, Table, "displaystyle", displaystyle, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Table, "equalcolumns", equalcolumns, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Table, "equalrows", equalrows, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Table, "frame", frame, true, true, false, false, "none");
DEFINE_ATTRIBUTE(MathML, Table, "framespacing", framespacing, true, true, false, false, "0.4em 0.5ex");
DEFINE_ATTRIBUTE(MathML, Table, "groupalign", groupalign, true, false, false, false, "{ left }");
DEFINE_ATTRIBUTE(MathML, Table, "minlabelspacing", minlabelspacing, true, true, false, false, "0.8em");
DEFINE_ATTRIBUTE(MathML, Table, "rowalign", rowalign, true, false, false, false, "baseline");
DEFINE_ATTRIBUTE(MathML, Table, "rowlines", rowlines, true, true, false, false, "none");
DEFINE_ATTRIBUTE(MathML, Table, "rowspacing", rowspacing, true, true, false, false, "1.0ex");
DEFINE_ATTRIBUTE(MathML, Table, "side", side, true, true, false, false, "right");
DEFINE_ATTRIBUTE(MathML, Table, "width", width, true, false, false, false, "auto");
DEFINE_ATTRIBUTE(MathML, TableCell, "columnalign", columnalign, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, TableCell, "columnspan", columnspan, true, true, false, false, "1");
DEFINE_ATTRIBUTE(MathML, TableCell, "groupalign", groupalign, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, TableCell, "rowalign", rowalign, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, TableCell, "rowspan", rowspan, true, true, false, false, "1");
DEFINE_ATTRIBUTE(MathML, TableRow, "columnalign", columnalign, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, TableRow, "groupalign", groupalign, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, TableRow, "rowalign", rowalign, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Token, "mathbackground", mathbackground, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Token, "mathcolor", mathcolor, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Token, "mathsize", mathsize, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Token, "mathvariant", mathvariant, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, UnderOver, "accent", accent, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, UnderOver, "accentunder", accentunder, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, math, "display", display, true, false, false, false, "inline");
DEFINE_ATTRIBUTE(MathML, math, "mode", mode, true, false, false, false, "inline");

