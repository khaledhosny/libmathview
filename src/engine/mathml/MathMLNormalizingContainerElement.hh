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

#ifndef __MathMLNormalizingContainerElement_hh__
#define __MathMLNormalizingContainerElement_hh__

#include "MathMLBinContainerElement.hh"

// base class for MathML elements that infer an mrow when the number of
// children is not 1
class GMV_MathView_EXPORT MathMLNormalizingContainerElement : public MathMLBinContainerElement
{
protected:
  MathMLNormalizingContainerElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLNormalizingContainerElement();

public:
  virtual AreaRef format(class FormattingContext&);

  virtual void setDirtyStructure(void);
};

#endif // __MathMLNormalizingContainerElement_hh__
