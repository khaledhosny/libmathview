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
#include <stdio.h>

#include "MathMLStringNode.hh"
#include "MathMLTokenElement.hh"
#include "RenderingEnvironment.hh"

MathMLStringNode::MathMLStringNode(const String* c)
{
  assert(c != NULL);
  content = c;
  fContent = NULL;
}

MathMLStringNode::~MathMLStringNode()
{
  delete content;
  delete fContent;
}

bool
MathMLStringNode::IsString() const
{
  return true;
}

void
MathMLStringNode::Setup(RenderingEnvironment& env)
{
  assert(content != NULL);

  if (fContent != NULL) delete fContent;
  fContent = new FontifiedString(*content, env.GetFontAttributes(), env.charMapper);
}

void
MathMLStringNode::DoLayout(const FormattingContext&)
{
  assert(fContent != NULL);
  fContent->GetBoundingBox(box);
}

void
MathMLStringNode::Render(const DrawingArea& area)
{
  assert(GetParent());
  assert(is_a<MathMLTokenElement>(GetParent()));
  assert(fContent != NULL);

  Ptr<MathMLTokenElement> token = smart_cast<MathMLTokenElement>(GetParent());
  assert(token);

  const GraphicsContext* gc = token->GetForegroundGC();
  fContent->Draw(GetX(), GetY(), area, gc);
}

bool
MathMLStringNode::HasDecimalPoint() const
{
  assert(content != NULL);

  for (unsigned i = 0; i < content->GetLength(); i++)
    if (content->GetChar(i) == '.') return true;

  return false;
}

scaled
MathMLStringNode::GetDecimalPointEdge() const
{
  assert(content != NULL);
  assert(GetParent());
  assert(is_a<MathMLTokenElement>(GetParent()));

  Ptr<MathMLTokenElement> parent = smart_cast<MathMLTokenElement>(GetParent());
  assert(parent);

  // let's find the position of the decimal point
  unsigned i;
  for (i = 0; i < content->GetLength() && content->GetChar(i) != '.'; i++) ;

  return GetX() + fContent->WidthTo(i);
}

unsigned
MathMLStringNode::GetLogicalContentLength() const
{
  assert(content != NULL);
  return content->GetLength();
}

String*
MathMLStringNode::GetRawContent() const
{
  assert(content != NULL);
  return content->Clone();
}
