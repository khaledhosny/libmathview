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

#ifndef __MathMLTextNode_hh__
#define __MathMLTextNode_hh__

#include "Area.hh"
#include "String.hh"
#include "MathMLNode.hh"

class GMV_MathView_EXPORT MathMLTextNode : public MathMLNode
{
protected:
  MathMLTextNode(void);
  virtual ~MathMLTextNode();

public:
  virtual AreaRef format(class FormattingContext&) = 0;

  virtual String   GetRawContent(void) const { return String(); }
  virtual unsigned GetLogicalContentLength(void) const { return 0; }
};

#endif // __MathMLTextNode_hh__
