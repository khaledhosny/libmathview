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
