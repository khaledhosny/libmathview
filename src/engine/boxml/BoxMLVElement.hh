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

#ifndef __BoxMLVElement_hh__
#define __BoxMLVElement_hh__

#include "BoxMLLinearContainerElement.hh"

class GMV_MathView_EXPORT BoxMLVElement : public BoxMLLinearContainerElement
{
protected:
  BoxMLVElement(const SmartPtr<class BoxMLNamespaceContext>&);
  virtual ~BoxMLVElement();

public:
  static SmartPtr<BoxMLVElement> create(const SmartPtr<class BoxMLNamespaceContext>&);

  virtual AreaRef format(class FormattingContext&);

  static AreaRef indentArea(const class FormattingContext&,
			    const AreaRef&, const AreaRef&);
  static scaled getMinimumIndentation(const class FormattingContext&,
				      const SmartPtr<Value>&);
  static AreaRef formatVerticalArray(class FormattingContext&,
				     const std::vector<AreaRef>&,
				     const scaled&, int, int,
				     const scaled&);
};

#endif // __BoxMLVElement_hh__
