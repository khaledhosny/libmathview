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
#include "MathMLFractionElement.hh"
#include "MathMLIdentifierElement.hh"
#include "MathMLInvisibleTimesNode.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "traverseAux.hh"

String
MathMLInvisibleTimesNode::getSpace(const FormattingContext& ctxt)
{
  static const String noSpace = StringOfUCS4String(UCS4String(1, 0x200b));
  static const String someSpace = StringOfUCS4String(UCS4String(1, 0x205f));

  // THESE CONSTANTS SHOULD BE CHECKED ON SOME MANUAL
  if (SmartPtr<MathMLOperatorElement> op = smart_cast<MathMLOperatorElement>(ctxt.getMathMLElement()))
    {
      SmartPtr<MathMLElement> prev = findLeftSibling(op);
      SmartPtr<MathMLElement> next = findRightSibling(op);
      if (!prev || !next) return noSpace;

      if (is_a<MathMLIdentifierElement>(prev) && is_a<MathMLIdentifierElement>(next))
	{
	  SmartPtr<MathMLTokenElement> prevToken = smart_cast<MathMLTokenElement>(prev);
	  SmartPtr<MathMLTokenElement> nextToken = smart_cast<MathMLTokenElement>(next);
	  assert(prevToken && nextToken);
    
	  if (prevToken->GetLogicalContentLength() <= 1 &&
	      nextToken->GetLogicalContentLength() <= 1) return noSpace;

	  return someSpace;
	} 
      else if (is_a<MathMLIdentifierElement>(prev))
	return someSpace;
      else if (is_a<MathMLFractionElement>(prev) && is_a<MathMLFractionElement>(next))
	return someSpace;
      else if (is_a<MathMLFractionElement>(prev) || is_a<MathMLFractionElement>(next))
	return someSpace;
    }

  return noSpace;
}

AreaRef
MathMLInvisibleTimesNode::format(FormattingContext& ctxt)
{ return ctxt.MGD()->string(ctxt, getSpace(ctxt)); }

String
MathMLInvisibleTimesNode::getContent()
{
  static const String content = StringOfUCS4String(UCS4String(1, 0x2062));
  return content;
}

String
MathMLInvisibleTimesNode::GetRawContent() const
{ return getContent(); }
