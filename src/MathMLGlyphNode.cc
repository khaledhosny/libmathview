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
#include <assert.h>
#include <string.h>

#include "Globals.hh"
#include "CharMapper.hh"
#include "MathMLElement.hh"
#include "MathMLGlyphNode.hh"
#include "MathMLTokenElement.hh"
#include "RenderingEnvironment.hh"

MathMLGlyphNode::MathMLGlyphNode(const char* n, const char* f, char nc)
{
  assert(n != NULL);
  assert(f != NULL);
  assert(strlen(n) > 0);
  assert(strlen(f) > 0);

  name = new char[strlen(n)];
  fontFamily = new char[strlen(f)];

  strcpy(name, n);
  strcpy(fontFamily, f);

  nch = nc;

  font = NULL;
}

MathMLGlyphNode::~MathMLGlyphNode()
{
  delete [] name;
  delete [] fontFamily;
}

void
MathMLGlyphNode::Setup(RenderingEnvironment& env)
{
  FontAttributes glyphAttributes = env.GetFontAttributes();
  glyphAttributes.family = fontFamily;
  glyphAttributes.mode = FONT_MODE_ANY;
  font = env.charMapper.GetFont(glyphAttributes);
}

void
MathMLGlyphNode::DoLayout(const FormattingContext&)
{
  if (font != NULL) font->CharBox(nch, box);
  else box.Null();
}

void
MathMLGlyphNode::Render(const DrawingArea& area)
{
  if (font != NULL)
    {
      assert(GetParent());
      assert(is_a<MathMLTokenElement>(GetParent()));
      Ptr<MathMLTokenElement> token = smart_cast<MathMLTokenElement>(GetParent());
      assert(token);

      const GraphicsContext* gc = token->GetForegroundGC();

      area.DrawChar(gc, font, GetX(), GetY(), nch);
    }
}

unsigned
MathMLGlyphNode::GetLogicalContentLength() const
{
  return 1;
}

String*
MathMLGlyphNode::GetRawContent() const
{
  return NULL;
}
