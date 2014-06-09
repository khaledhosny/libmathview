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

#ifndef __MathMLAlignGroupElement_hh__
#define __MathMLAlignGroupElement_hh__

#include "MathMLElement.hh"

class MathMLAlignGroupElement : public MathMLElement
{
protected:
  MathMLAlignGroupElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLAlignGroupElement();

public:
  static SmartPtr<MathMLAlignGroupElement> create(const SmartPtr<class MathMLNamespaceContext>& context)
  { return new MathMLAlignGroupElement(context); }

  void SetWidth(const scaled&);
  void SetDecimalPoint(const SmartPtr<class MathMLTokenElement>&);
  void SetAlignmentMark(const SmartPtr<class MathMLMarkNode>&);
  void SetAlignmentMark(const SmartPtr<class MathMLAlignMarkElement>&);

  const SmartPtr<class MathMLMarkNode>& GetAlignmentMarkNode(void) const { return alignMarkNode; }
  const SmartPtr<class MathMLAlignMarkElement>& GetAlignmentMarkElement(void) const { return alignMarkElement; }
  const SmartPtr<class MathMLTokenElement>& GetDecimalPoint(void) const { return decimalPoint; }

  virtual bool IsSpaceLike(void) const;

private:
  scaled width;

  SmartPtr<class MathMLMarkNode>         alignMarkNode;
  SmartPtr<class MathMLAlignMarkElement> alignMarkElement;
  SmartPtr<class MathMLTokenElement>     decimalPoint;
};

#endif // __MathMLAlignGroupElement_hh__
