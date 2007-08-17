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

#ifndef __BoxMLTextElement_hh__
#define __BoxMLTextElement_hh__

#include "BoxMLElement.hh"

class GMV_MathView_EXPORT BoxMLTextElement : public BoxMLElement
{
protected:
  BoxMLTextElement(const SmartPtr<class BoxMLNamespaceContext>&);
  virtual ~BoxMLTextElement();

public:
  static SmartPtr<BoxMLTextElement> create(const SmartPtr<class BoxMLNamespaceContext>&);

  virtual AreaRef format(class FormattingContext&);

  void setContent(const String&);
  String getContent(void) const { return content; }

private:
  String content;
};

#endif // __BoxMLTextElement_hh__
