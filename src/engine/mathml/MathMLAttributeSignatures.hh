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

#ifndef __MathMLAttributeSignatures_hh__
#define __MathMLAttributeSignatures_hh__

#include "AttributeSignature.hh"

DECLARE_ATTRIBUTE(MathML, Action, actiontype);
DECLARE_ATTRIBUTE(MathML, Action, selection);
DECLARE_ATTRIBUTE(MathML, AlignGroup, groupalign);
DECLARE_ATTRIBUTE(MathML, AlignMark, edge);
DECLARE_ATTRIBUTE(MathML, Element, id);
DECLARE_ATTRIBUTE(MathML, Element, class);
DECLARE_ATTRIBUTE(MathML, Element, other);
DECLARE_ATTRIBUTE(MathML, Enclose, notation);
DECLARE_ATTRIBUTE(MathML, Fenced, open);
DECLARE_ATTRIBUTE(MathML, Fenced, close);
DECLARE_ATTRIBUTE(MathML, Fenced, separators);
DECLARE_ATTRIBUTE(MathML, Fraction, linethickness);
DECLARE_ATTRIBUTE(MathML, Fraction, numalign);
DECLARE_ATTRIBUTE(MathML, Fraction, denomalign);
DECLARE_ATTRIBUTE(MathML, Fraction, bevelled);
DECLARE_ATTRIBUTE(MathML, MultiScripts, subscriptshift);
DECLARE_ATTRIBUTE(MathML, MultiScripts, superscriptshift);
DECLARE_ATTRIBUTE(MathML, Script, subscriptshift);
DECLARE_ATTRIBUTE(MathML, Script, superscriptshift);
DECLARE_ATTRIBUTE(MathML, Operator, form);
DECLARE_ATTRIBUTE(MathML, Operator, fence);
DECLARE_ATTRIBUTE(MathML, Operator, separator);
DECLARE_ATTRIBUTE(MathML, Operator, lspace);
DECLARE_ATTRIBUTE(MathML, Operator, rspace);
DECLARE_ATTRIBUTE(MathML, Operator, stretchy);
DECLARE_ATTRIBUTE(MathML, Operator, symmetric);
DECLARE_ATTRIBUTE(MathML, Operator, maxsize);
DECLARE_ATTRIBUTE(MathML, Operator, minsize);
DECLARE_ATTRIBUTE(MathML, Operator, largeop);
DECLARE_ATTRIBUTE(MathML, Operator, movablelimits);
DECLARE_ATTRIBUTE(MathML, Operator, accent);
DECLARE_ATTRIBUTE(MathML, Padded, width);
DECLARE_ATTRIBUTE(MathML, Padded, lspace);
DECLARE_ATTRIBUTE(MathML, Padded, height);
DECLARE_ATTRIBUTE(MathML, Padded, depth);
DECLARE_ATTRIBUTE(MathML, Space, width);
DECLARE_ATTRIBUTE(MathML, Space, height);
DECLARE_ATTRIBUTE(MathML, Space, depth);
DECLARE_ATTRIBUTE(MathML, Space, linebreak);
DECLARE_ATTRIBUTE(MathML, StringLit, lquote);
DECLARE_ATTRIBUTE(MathML, StringLit, rquote);
DECLARE_ATTRIBUTE(MathML, Style, scriptlevel);
DECLARE_ATTRIBUTE(MathML, Style, displaystyle);
DECLARE_ATTRIBUTE(MathML, Style, scriptsizemultiplier);
DECLARE_ATTRIBUTE(MathML, Style, scriptminsize);
DECLARE_ATTRIBUTE(MathML, Style, color);
DECLARE_ATTRIBUTE(MathML, Style, background);
DECLARE_ATTRIBUTE(MathML, Style, mathcolor);
DECLARE_ATTRIBUTE(MathML, Style, mathbackground);
DECLARE_ATTRIBUTE(MathML, Style, veryverythinmathspace);
DECLARE_ATTRIBUTE(MathML, Style, verythinmathspace);
DECLARE_ATTRIBUTE(MathML, Style, thinmathspace);
DECLARE_ATTRIBUTE(MathML, Style, mediummathspace);
DECLARE_ATTRIBUTE(MathML, Style, thickmathspace);
DECLARE_ATTRIBUTE(MathML, Style, verythickmathspace);
DECLARE_ATTRIBUTE(MathML, Style, veryverythickmathspace);
DECLARE_ATTRIBUTE(MathML, Style, negativeveryverythinmathspace);
DECLARE_ATTRIBUTE(MathML, Style, negativeverythinmathspace);
DECLARE_ATTRIBUTE(MathML, Style, negativethinmathspace);
DECLARE_ATTRIBUTE(MathML, Style, negativemediummathspace);
DECLARE_ATTRIBUTE(MathML, Style, negativethickmathspace);
DECLARE_ATTRIBUTE(MathML, Style, negativeverythickmathspace);
DECLARE_ATTRIBUTE(MathML, Style, negativeveryverythickmathspace);
DECLARE_ATTRIBUTE(MathML, Style, mathsize);
DECLARE_ATTRIBUTE(MathML, Style, mathcolor);
DECLARE_ATTRIBUTE(MathML, TableCell, rowspan);
DECLARE_ATTRIBUTE(MathML, TableCell, columnspan);
DECLARE_ATTRIBUTE(MathML, TableCell, rowalign);
DECLARE_ATTRIBUTE(MathML, TableCell, columnalign);
DECLARE_ATTRIBUTE(MathML, TableCell, groupalign);
DECLARE_ATTRIBUTE(MathML, TableRow, rowalign);
DECLARE_ATTRIBUTE(MathML, TableRow, columnalign);
DECLARE_ATTRIBUTE(MathML, TableRow, groupalign);
DECLARE_ATTRIBUTE(MathML, Table, align);
DECLARE_ATTRIBUTE(MathML, Table, alignmentscope);
DECLARE_ATTRIBUTE(MathML, Table, columnwidth);
DECLARE_ATTRIBUTE(MathML, Table, rowspacing);
DECLARE_ATTRIBUTE(MathML, Table, columnspacing);
DECLARE_ATTRIBUTE(MathML, Table, rowlines);
DECLARE_ATTRIBUTE(MathML, Table, columnlines);
DECLARE_ATTRIBUTE(MathML, Table, frame);
DECLARE_ATTRIBUTE(MathML, Table, framespacing);
DECLARE_ATTRIBUTE(MathML, Table, equalrows);
DECLARE_ATTRIBUTE(MathML, Table, equalcolumns);
DECLARE_ATTRIBUTE(MathML, Table, displaystyle);
DECLARE_ATTRIBUTE(MathML, Table, side);
DECLARE_ATTRIBUTE(MathML, Table, minlabelspacing);
DECLARE_ATTRIBUTE(MathML, Table, width);
DECLARE_ATTRIBUTE(MathML, Table, rowalign);
DECLARE_ATTRIBUTE(MathML, Table, columnalign);
DECLARE_ATTRIBUTE(MathML, Table, groupalign);
DECLARE_ATTRIBUTE(MathML, Token, mathvariant);
DECLARE_ATTRIBUTE(MathML, Token, mathsize);
DECLARE_ATTRIBUTE(MathML, Token, mathcolor);
DECLARE_ATTRIBUTE(MathML, Token, mathbackground);
DECLARE_ATTRIBUTE(MathML, UnderOver, accentunder);
DECLARE_ATTRIBUTE(MathML, UnderOver, accent);
DECLARE_ATTRIBUTE(MathML, math, mode);
DECLARE_ATTRIBUTE(MathML, math, display);

#endif // __MathMLAttributeSignatures_hh__

