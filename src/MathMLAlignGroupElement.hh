// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
// 
// This file is part of GtkMathView, a Gtk widget for MathML.
// 
// GtkMathView is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with GtkMathView; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// For details, see the GtkMathView World-Wide-Web page,
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef MathMLAlignGroupElement_hh
#define MathMLAlignGroupElement_hh

#if defined(HAVE_MINIDOM)
#include <minidom.h>
#elif defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLElement.hh"

class MathMLAlignGroupElement: public MathMLElement
{
public:
#if defined(HAVE_MINIDOM)
  MathMLAlignGroupElement(mDOMNodeRef);
#elif defined(HAVE_GMETADOM)
  MathMLAlignGroupElement(const GMetaDOM::Element&);
#endif
  virtual void DoBoxedLayout(LayoutId, BreakId, scaled);
  virtual ~MathMLAlignGroupElement();

  void SetWidth(scaled);
  void SetDecimalPoint(class MathMLTokenElement*);
  void SetAlignmentMark(class MathMLMarkNode*);
  void SetAlignmentMark(class MathMLAlignMarkElement*);

  class MathMLMarkNode* GetAlignmentMarkNode(void) const { return alignMarkNode; }
  class MathMLAlignMarkElement* GetAlignmentMarkElement(void) const { return alignMarkElement; }
  class MathMLTokenElement* GetDecimalPoint(void) const { return decimalPoint; }

  virtual bool IsSpaceLike(void) const;

private:
  scaled width;

  class MathMLMarkNode*         alignMarkNode;
  class MathMLAlignMarkElement* alignMarkElement;
  class MathMLTokenElement*     decimalPoint;
};

#endif // MathMLAlignGroupElement_hh
