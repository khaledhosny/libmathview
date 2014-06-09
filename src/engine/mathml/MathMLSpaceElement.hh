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

#ifndef __MathMLSpaceElement_hh__
#define __MathMLSpaceElement_hh__

#include "token.hh"
#include "MathMLElement.hh"

class MathMLSpaceElement : public MathMLElement
{
protected:
  MathMLSpaceElement(const SmartPtr<class MathMLNamespaceContext>&);
  virtual ~MathMLSpaceElement();

public:
  static SmartPtr<MathMLSpaceElement> create(const SmartPtr<class MathMLNamespaceContext>& view)
  { return new MathMLSpaceElement(view); }

  virtual AreaRef format(class FormattingContext&);

  virtual bool    IsSpace(void) const;
  virtual bool    IsSpaceLike(void) const;
  TokenId         GetBreakability(void) const { return breakability; }

private:
  bool    lineBreak;
  bool    autoLineBreak; // valid if lineBreaking == true
  TokenId breakability; // valid if auto == false
};

#endif // __MathMLSpaceElement_hh__
