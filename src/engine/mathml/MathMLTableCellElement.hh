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

#ifndef __MathMLTableCellElement_hh__
#define __MathMLTableCellElement_hh__

#include "token.hh"
#include "MathMLNormalizingContainerElement.hh"

class GMV_MathView_EXPORT MathMLTableCellElement : public MathMLNormalizingContainerElement
{
protected:
  MathMLTableCellElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLTableCellElement();

public:
  static SmartPtr<MathMLTableCellElement> create(const SmartPtr<class MathMLNamespaceContext>& view)
  { return new MathMLTableCellElement(view); }

  //bool     IsStretchyOperator(void) const;

  //friend class MathMLTableElement;
  //friend class MathMLTableRowElement;

  virtual void setDirtyAttribute(void);
  virtual void setDirtyStructure(void);
  virtual void setDirtyLayout(void);

  unsigned getRowIndex(void) const { return rowIndex; }
  unsigned getColumnIndex(void) const { return columnIndex; }
  unsigned getRowSpan(void) const { return rowSpan; }
  unsigned getColumnSpan(void) const { return columnSpan; }
  void setSpan(unsigned, unsigned);
  void setPosition(unsigned, unsigned);
  void setDisplacement(const scaled&, const scaled&);
  void getDisplacement(scaled&, scaled&) const;
  void setAlignment(TokenId, TokenId);
  TokenId getRowAlign(void) const;
  TokenId getColumnAlign(void) const;
  //void setGroupAlignment(const std::vector<TokenId>&);

private:
  scaled dispX;
  scaled dispY;
  unsigned rowIndex;
  unsigned columnIndex;
  unsigned rowSpan;
  unsigned columnSpan;
  TokenId rowAlign;
  TokenId columnAlign;
};

#endif // __MathMLTableCellElement_hh__
