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

#ifndef traverseAux_hh
#define traverseAux_hh

#include <minidom.h>

#include "CharMap.hh"

class MathMLElement*         findEmbellishedOperatorRoot(class MathMLElement*);
class MathMLOperatorElement* findCoreOperator(class MathMLElement*);
class MathMLOperatorElement* findStretchyOperator(class MathMLElement*);
class MathMLOperatorElement* findStretchyOperator(class MathMLElement*, StretchId);
class MathMLElement*         findCommonAncestor(class MathMLElement*, class MathMLElement*);
class MathMLActionElement*   findActionElement(class MathMLElement*);
mDOMNodeRef                  findDOMNode(class MathMLElement*);
class MathMLElement*         getMathMLElement(mDOMNodeRef);
class MathMLElement*         findMathMLElement(mDOMNodeRef);
class MathMLElement*         findRightmostChild(class MathMLElement*);
class MathMLElement*         findLeftmostChild(class MathMLElement*);
class MathMLElement*         findRightSibling(class MathMLElement*);
class MathMLElement*         findLeftSibling(class MathMLElement*);

#endif // traverseAux_hh
