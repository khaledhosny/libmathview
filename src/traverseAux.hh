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

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "Ptr.hh"
#include "CharMap.hh"

Ptr<class MathMLElement>         findEmbellishedOperatorRoot(const Ptr<class MathMLElement>&);
Ptr<class MathMLOperatorElement> findStretchyOperator(const Ptr<class MathMLElement>&);
Ptr<class MathMLOperatorElement> findStretchyOperator(const Ptr<class MathMLElement>&, StretchId);
Ptr<class MathMLElement>         findCommonAncestor(const Ptr<class MathMLElement>&,
						    const Ptr<class MathMLElement>&);
Ptr<class MathMLActionElement>   findActionElement(const Ptr<class MathMLElement>&);
#if defined(HAVE_GMETADOM)
DOM::Element                findDOMNode(const Ptr<class MathMLElement>&);
Ptr<class MathMLElement>         findMathMLElement(const Ptr<class MathMLDocument>&,
						   const DOM::Element&);
#endif
Ptr<class MathMLElement>         findRightmostChild(const Ptr<class MathMLElement>&);
Ptr<class MathMLElement>         findLeftmostChild(const Ptr<class MathMLElement>&);
Ptr<class MathMLElement>         findRightSibling(const Ptr<class MathMLElement>&);
Ptr<class MathMLElement>         findLeftSibling(const Ptr<class MathMLElement>&);

#endif // traverseAux_hh
