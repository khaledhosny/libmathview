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

#ifndef __MathMLTokenElement_hh__
#define __MathMLTokenElement_hh__

#include "MathMLElement.hh"
#include "MathMLTextNode.hh"
#include "LinearContainerTemplate.hh"

// base class for token element. Token elemens can contain character data
// and a very limited set of other MathML elements (e.g. <malignmark>)
class GMV_MathView_EXPORT MathMLTokenElement : public MathMLElement
{
protected:
  MathMLTokenElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLTokenElement();

public:
  unsigned getSize(void) const { return content.getSize(); }
  void setSize(unsigned i) { content.setSize(this, i); }
  SmartPtr<class MathMLTextNode> getChild(unsigned i) const { return content.getChild(i); }
  void setChild(unsigned i, const SmartPtr<class MathMLTextNode>& child) { content.setChild(this, i, child); }
  void appendChild(const SmartPtr<class MathMLTextNode>& child) { content.appendChild(this, child); }
  void append(const String&);
  void swapContent(std::vector< SmartPtr<MathMLTextNode> >& newContent) { content.swapContent(this, newContent); }
  const std::vector< SmartPtr<class MathMLTextNode> >& getContent(void) const { return content.getContent(); }

  virtual AreaRef format(class FormattingContext&);

  bool           IsNonMarking(void) const;

  String         GetRawContent(void) const;
  unsigned       GetLogicalContentLength(void) const;
  unsigned getContentLength(void) const;

private:
  LinearContainerTemplate<MathMLTokenElement,class MathMLTextNode> content;

protected:
  AreaRef formatAux(class FormattingContext&);
  
  void AddItalicCorrection(void);
};

#endif // __MathMLTokenElement_hh__
