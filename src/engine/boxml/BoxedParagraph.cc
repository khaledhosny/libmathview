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

#include "BoxedParagraph.hh"
#include "BoxFormattingContext.hh"
#include "BoxMLElement.hh"

BoxedParagraph::BoxedParagraph()
  : objectReplacement(fromUCS4String(DOM::UCS4String(1, 0xfffc)))
{ }

void
BoxedParagraph::append(const BoxFormattingContext& ctxt, const String& s)
{
  if (!content.empty()) content.append(' ');
  object.push_back(Object(ctxt, getIndex(), s, 0));
  content.append(s.data(), s.length());
}

void
BoxedParagraph::append(const BoxFormattingContext& ctxt, const AreaRef& area)
{
  if (!content.empty()) content.append(' ');
  object.push_back(Object(ctxt, getIndex(), objectReplacement, area));
  content.append(objectReplacement.data(), objectReplacement.length());
}

BoxedParagraph::Object::Object(const BoxFormattingContext& ctxt,
			       int start,
			       const DOM::UTF8String& s,
			       const AreaRef& a)
  : start_index(start), end_index(start + s.length()), str(s), area(a),
    foreground(ctxt.getColor()),
    background(ctxt.getBackground()),
    size(ctxt.getSize())
{ }
