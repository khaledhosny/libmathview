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

#include <config.h>

#include "MathMLOperatorElement.hh"
#include "MathMLRowElement.hh"
#include "MathMLFunctionApplicationNode.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "traverseAux.hh"

String
MathMLFunctionApplicationNode::getSpace(const FormattingContext& ctxt)
{
  static const String noSpace = StringOfUCS4String(UCS4String(1, 0x200b));
  static const String someSpace = StringOfUCS4String(UCS4String(1, 0x205f));

  if (SmartPtr<MathMLOperatorElement> op = smart_cast<MathMLOperatorElement>(ctxt.getMathMLElement()))
    {
      SmartPtr<MathMLElement> next = findRightSibling(op);
      if (!next) return noSpace;

      if (SmartPtr<MathMLOperatorElement> coreOp = next->getCoreOperatorTop())
	if (coreOp->IsFence()) return noSpace;

      if (SmartPtr<MathMLRowElement> row = smart_cast<MathMLRowElement>(next))
	if (SmartPtr<MathMLOperatorElement> coreOp = smart_cast<MathMLOperatorElement>(row->getChild(0)))
	  if (coreOp->IsFence()) return noSpace;

      return someSpace;
    }

  return noSpace;
}

AreaRef
MathMLFunctionApplicationNode::format(FormattingContext& ctxt)
{ return ctxt.MGD()->string(ctxt, getSpace(ctxt)); }

String
MathMLFunctionApplicationNode::getContent()
{
  static const String content = StringOfUCS4String(UCS4String(1, 0x2061));
  return content;
}

String
MathMLFunctionApplicationNode::GetRawContent() const
{ return getContent(); }
