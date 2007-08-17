// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include <cassert>

#include "MathMLAttributeParsers.hh"
#include "MathMLAttributeSignatures.hh"

DEFINE_ATTRIBUTE(MathML, Action, actiontype, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Action, selection, true, true, false, false, "1");
DEFINE_ATTRIBUTE(MathML, AlignGroup, groupalign, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, AlignMark, edge, true, true, false, false, "left");
DEFINE_ATTRIBUTE(MathML, Element, class, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Element, id, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Element, other, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Enclose, notation, true, true, false, false, "longdiv");
DEFINE_ATTRIBUTE(MathML, Fenced, close, true, true, false, false, ")");
DEFINE_ATTRIBUTE(MathML, Fenced, open, true, true, false, false, "(");
DEFINE_ATTRIBUTE(MathML, Fenced, separators, true, true, false, true, ",");
DEFINE_ATTRIBUTE(MathML, Fraction, bevelled, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Fraction, denomalign, true, true, false, false, "center");
DEFINE_ATTRIBUTE(MathML, Fraction, linethickness, true, true, false, false, "1");
DEFINE_ATTRIBUTE(MathML, Fraction, numalign, true, true, false, false, "center");
DEFINE_ATTRIBUTE(MathML, Operator, accent, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Operator, fence, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Operator, form, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Operator, largeop, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Operator, lspace, true, true, false, false, "thickmathspace");
DEFINE_ATTRIBUTE(MathML, Operator, maxsize, true, true, false, false, "infinity");
DEFINE_ATTRIBUTE(MathML, Operator, minsize, true, true, false, false, "1");
DEFINE_ATTRIBUTE(MathML, Operator, movablelimits, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Operator, rspace, true, true, false, false, "thickmathspace");
DEFINE_ATTRIBUTE(MathML, Operator, separator, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Operator, stretchy, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Operator, symmetric, true, true, false, false, "true");
DEFINE_ATTRIBUTE(MathML, Padded, depth, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Padded, height, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Padded, lspace, true, false, false, false, "0em");
DEFINE_ATTRIBUTE(MathML, Padded, width, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, MultiScripts, subscriptshift, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, MultiScripts, superscriptshift, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Script, subscriptshift, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Script, superscriptshift, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Space, depth, true, true, false, false, "0ex");
DEFINE_ATTRIBUTE(MathML, Space, height, true, true, false, false, "0ex");
DEFINE_ATTRIBUTE(MathML, Space, linebreak, true, true, false, false, "auto");
DEFINE_ATTRIBUTE(MathML, Space, width, true, true, false, false, "0em");
DEFINE_ATTRIBUTE(MathML, StringLit, lquote, true, true, false, false, "\"");
DEFINE_ATTRIBUTE(MathML, StringLit, rquote, true, true, false, false, "\"");
DEFINE_ATTRIBUTE(MathML, Style, color, true, false, true, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, background, true, false, true, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, mathcolor, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, mathbackground, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, displaystyle, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, mathsize, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, mediummathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, negativemediummathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, negativethickmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, negativethinmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, negativeverythickmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, negativeverythinmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, negativeveryverythickmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, negativeveryverythinmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, scriptlevel, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, scriptminsize, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, scriptsizemultiplier, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, thickmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, thinmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, verythickmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, verythinmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, veryverythickmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Style, veryverythinmathspace, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Table, align, true, true, false, false, "axis");
DEFINE_ATTRIBUTE(MathML, Table, alignmentscope, true, true, false, false, "true");
DEFINE_ATTRIBUTE(MathML, Table, columnalign, true, false, false, false, "center");
DEFINE_ATTRIBUTE(MathML, Table, columnlines, true, true, false, false, "none");
DEFINE_ATTRIBUTE(MathML, Table, columnspacing, true, true, false, false, "0.8em");
DEFINE_ATTRIBUTE(MathML, Table, columnwidth, true, true, false, false, "auto");
DEFINE_ATTRIBUTE(MathML, Table, displaystyle, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Table, equalcolumns, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Table, equalrows, true, true, false, false, "false");
DEFINE_ATTRIBUTE(MathML, Table, frame, true, true, false, false, "none");
DEFINE_ATTRIBUTE(MathML, Table, framespacing, true, true, false, false, "0.4em 0.5ex");
DEFINE_ATTRIBUTE(MathML, Table, groupalign, true, false, false, false, "{ left }");
DEFINE_ATTRIBUTE(MathML, Table, minlabelspacing, true, true, false, false, "0.8em");
DEFINE_ATTRIBUTE(MathML, Table, rowalign, true, false, false, false, "baseline");
DEFINE_ATTRIBUTE(MathML, Table, rowlines, true, true, false, false, "none");
DEFINE_ATTRIBUTE(MathML, Table, rowspacing, true, true, false, false, "1.0ex");
DEFINE_ATTRIBUTE(MathML, Table, side, true, true, false, false, "right");
DEFINE_ATTRIBUTE(MathML, Table, width, true, false, false, false, "auto");
DEFINE_ATTRIBUTE(MathML, TableCell, columnalign, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, TableCell, columnspan, true, true, false, false, "1");
DEFINE_ATTRIBUTE(MathML, TableCell, groupalign, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, TableCell, rowalign, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, TableCell, rowspan, true, true, false, false, "1");
DEFINE_ATTRIBUTE(MathML, TableRow, columnalign, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, TableRow, groupalign, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, TableRow, rowalign, true, false, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Token, mathbackground, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Token, mathcolor, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Token, mathsize, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, Token, mathvariant, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, UnderOver, accent, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, UnderOver, accentunder, true, true, false, false, 0);
DEFINE_ATTRIBUTE(MathML, math, display, true, false, false, false, "inline");
DEFINE_ATTRIBUTE(MathML, math, mode, true, false, false, false, "inline");

