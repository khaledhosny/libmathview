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

