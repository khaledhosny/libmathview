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

#ifndef __BoxMLDecorElement_hh__
#define __BoxMLDecorElement_hh__

#include "String.hh"
#include "BoxMLBinContainerElement.hh"

class GMV_MathView_EXPORT BoxMLDecorElement : public BoxMLBinContainerElement
{
protected:
  BoxMLDecorElement(const SmartPtr<class BoxMLNamespaceContext>&);
  virtual ~BoxMLDecorElement();

public:
  static SmartPtr<BoxMLDecorElement> create(const SmartPtr<class BoxMLNamespaceContext>&);

  virtual AreaRef format(class FormattingContext&);

private:
  static AreaRef decorate(class FormattingContext&,
			  const AreaRef&, const scaled&,
			  const class RGBColor&, const String&);
};

#endif // __BoxMLDecorElement_hh__
