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

#ifndef __MathMLContainerElement_hh__
#define __MathMLContainerElement_hh__

#include "MathMLElement.hh"

// base class for every non-empty MathML container element
class GMV_MathView_EXPORT MathMLContainerElement : public MathMLElement
{
protected:
  MathMLContainerElement(const SmartPtr<class MathMLNamespaceContext>&);
};

#endif // __MathMLContainerElement_hh__
