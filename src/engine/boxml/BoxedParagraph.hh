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

#ifndef __BoxedParagraph_hh__
#define __BoxedParagraph_hh__

#include <vector>

#include "Area.hh"
#include "RGBColor.hh"
#include "String.hh"
#include "Rope.hh"

class BoxedParagraph
{
public:
  BoxedParagraph(void);

  void append(const class FormattingContext&, const String&);
  void append(const class FormattingContext&, const AreaRef&);

  struct Object
  {
    Object(const class FormattingContext&, int, const String&, const AreaRef&);

    int start_index;
    int end_index;
    UTF8String str;
    AreaRef area;
    RGBColor foreground;
    RGBColor background;
    scaled size;
  };

  int getIndex(void) const { return content.size(); }

  std::vector<Object>::const_iterator begin(void) const { return object.begin(); }
  std::vector<Object>::const_iterator end(void) const { return object.end(); }

  UTF8String getContent(void) const { return UTF8String(content.begin(), content.end()); }

private:
  const UTF8String objectReplacement;
  std::vector<Object> object;
  ROPE_NS::rope<Char8> content;
};

#endif // __BoxedParagraph_hh__

