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

#include <config.h>

#include <cassert>

#include "MathMLElement.hh"
#include "MathMLStringNode.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"

inline bool
isCombining(Char32 ch)
{
  return (ch >= 0x0300 && ch <= 0x0362) || (ch >= 0x20d0 && ch <= 0x20e8);
}

MathMLStringNode::MathMLStringNode(const String& c)
  : content(c)
{ }

MathMLStringNode::~MathMLStringNode()
{ }

AreaRef
MathMLStringNode::format(FormattingContext& ctxt)
{ return ctxt.MGD()->string(ctxt, content); }

unsigned
MathMLStringNode::GetLogicalContentLength() const
{
  UCS4String s = UCS4StringOfString(content);

  unsigned length = 0;
  for (UCS4String::const_iterator i = s.begin(); i != s.end(); i++)
    {
      if (!isCombining(*i) || i == s.begin())
	length++;
    }

  return length;
}

String
MathMLStringNode::GetRawContent() const
{ return content; }
