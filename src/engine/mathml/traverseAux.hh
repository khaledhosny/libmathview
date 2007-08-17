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

#ifndef __traverseAux_hh__
#define __traverseAux_hh__

#include "SmartPtr.hh"

SmartPtr<class MathMLElement>         findEmbellishedOperatorRoot(const SmartPtr<class MathMLElement>&);
SmartPtr<class MathMLOperatorElement> findStretchyOperator(const SmartPtr<class MathMLElement>&);
SmartPtr<class MathMLElement>         findCommonAncestor(const SmartPtr<class MathMLElement>&,
							 const SmartPtr<class MathMLElement>&);
SmartPtr<class MathMLActionElement>   findActionElement(const SmartPtr<class MathMLElement>&);
#if 0
DOM::Element                findDOMNode(const SmartPtr<class MathMLElement>&);
SmartPtr<class MathMLElement>         findMathMLElement(const SmartPtr<const class MathMLView>&,
							const DOM::Element&);
#endif
SmartPtr<class MathMLElement>         findRightmostChild(const SmartPtr<class MathMLElement>&);
SmartPtr<class MathMLElement>         findLeftmostChild(const SmartPtr<class MathMLElement>&);
SmartPtr<class MathMLElement>         findRightSibling(const SmartPtr<class MathMLElement>&);
SmartPtr<class MathMLElement>         findLeftSibling(const SmartPtr<class MathMLElement>&);

#endif // __traverseAux_hh__

