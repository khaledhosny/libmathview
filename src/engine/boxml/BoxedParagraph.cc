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

#include "String.hh"
#include "BoxedParagraph.hh"
#include "FormattingContext.hh"
#include "BoxMLElement.hh"

BoxedParagraph::BoxedParagraph()
  : objectReplacement(StringOfUCS4String(UCS4String(1, 0xfffc)))
{ }

void
BoxedParagraph::append(const FormattingContext& ctxt, const String& s)
{
  if (!content.empty()) content.append(' ');
  object.push_back(Object(ctxt, getIndex(), s, 0));
  content.append(s.data(), s.length());
}

void
BoxedParagraph::append(const FormattingContext& ctxt, const AreaRef& area)
{
  if (!content.empty()) content.append(' ');
  object.push_back(Object(ctxt, getIndex(), objectReplacement, area));
  content.append(objectReplacement.data(), objectReplacement.length());
}

BoxedParagraph::Object::Object(const FormattingContext& ctxt,
			       int start,
			       const UTF8String& s,
			       const AreaRef& a)
  : start_index(start), end_index(start + s.length()), str(s), area(a),
    foreground(ctxt.getColor()),
    background(ctxt.getBackground()),
    size(ctxt.getSize())
{ }
