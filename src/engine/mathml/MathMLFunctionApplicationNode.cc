// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

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
  static const String zeroWidthSpace = StringOfUCS4String(UCS4String(0x200b, 1));
  static const String mediumMathSpace = StringOfUCS4String(UCS4String(0x205f, 1));

  if (SmartPtr<MathMLOperatorElement> op = smart_cast<MathMLOperatorElement>(ctxt.getMathMLElement()))
    {
      SmartPtr<MathMLElement> next = findRightSibling(op);
      if (!next) return zeroWidthSpace;

      if (SmartPtr<MathMLOperatorElement> coreOp = next->getCoreOperatorTop())
	if (coreOp->IsFence()) return zeroWidthSpace;

      if (SmartPtr<MathMLRowElement> row = smart_cast<MathMLRowElement>(next))
	if (SmartPtr<MathMLOperatorElement> coreOp = smart_cast<MathMLOperatorElement>(row->getChild(0)))
	  if (coreOp->IsFence()) return zeroWidthSpace;

      return mediumMathSpace;
    }

  return zeroWidthSpace;
}

AreaRef
MathMLFunctionApplicationNode::format(FormattingContext& ctxt)
{ return ctxt.MGD()->string(ctxt, getSpace(ctxt)); }

String
MathMLFunctionApplicationNode::getContent()
{
  static const String content = StringOfUCS4String(UCS4String(0x2061, 1));
  return content;
}

String
MathMLFunctionApplicationNode::GetRawContent() const
{ return getContent(); }
