// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#include <cassert>

#include "MathMLElement.hh"
#include "MathMLGlyphNode.hh"
#include "MathMLTokenElement.hh"

MathMLGlyphNode::MathMLGlyphNode(const String& f, const String& i, const String& a)
  : family(f), index(i), alt(a)
{
}

MathMLGlyphNode::~MathMLGlyphNode()
{
}

#if 0
void
MathMLGlyphNode::Setup(RenderingEnvironment& env)
{
//   FontAttributes glyphAttributes = env.GetFontAttributes();
//   glyphAttributes.family = fontFamily;
//   glyphAttributes.mode = FONT_MODE_ANY;
  //font = env.charMapper.GetFont(glyphAttributes);
}

void
MathMLGlyphNode::DoLayout(const FormattingContext&)
{
#if 0
  if (font) font->CharBox(nch, box);
  else box.unset();
#endif
}
#endif

AreaRef
MathMLGlyphNode::format(MathFormattingContext&)
{
  assert(false);
  return 0;
}

#if 0
void
MathMLGlyphNode::Render(const DrawingArea& area)
{
#if 0
  if (font)
    {
      assert(GetParent());
      assert(is_a<MathMLTokenElement>(GetParent()));
      SmartPtr<MathMLTokenElement> token = smart_cast<MathMLTokenElement>(GetParent());
      assert(token);

      const GraphicsContext* gc = token->GetForegroundGC();

      area.DrawChar(gc, font, GetX(), GetY(), nch);
    }
#endif
}
#endif

unsigned
MathMLGlyphNode::GetLogicalContentLength() const
{
  return 1;
}
