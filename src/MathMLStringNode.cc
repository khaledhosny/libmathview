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

#include "stringAux.hh"
#include "MathMLStringNode.hh"
#include "MathMLTokenElement.hh"
#include "RenderingEnvironment.hh"

#include "Gtk_DrawingArea.hh"
#include "ShaperManager.hh"
#include "Gtk_RenderingContext.hh"

unsigned MathMLStringNode::visited = 0;

MathMLStringNode::MathMLStringNode(const DOM::GdomeString& c)
{
  content = c;
}

MathMLStringNode::~MathMLStringNode()
{
}

bool
MathMLStringNode::IsString() const
{
  return true;
}

void
MathMLStringNode::Setup(RenderingEnvironment& env)
{
  area = env.shaperManager.shape(env.areaFactory, content, env.GetFontAttributes().size.ToScaledPoints());
}

void
MathMLStringNode::DoLayout(const FormattingContext&)
{
  box = area->box();
}

void
MathMLStringNode::Render(const DrawingArea& a)
{
  assert(GetParent());
  assert(is_a<MathMLTokenElement>(GetParent()));

  SmartPtr<MathMLTokenElement> token = smart_cast<MathMLTokenElement>(GetParent());
  assert(token);

  const GraphicsContext* gc = token->GetForegroundGC();

  const Gtk_DrawingArea& gtk_area = dynamic_cast<const Gtk_DrawingArea&>(a);
  Gtk_RenderingContext rc;
  rc.setDrawable(gtk_area.GetPixmap());
  
  assert(area);
  area->render(rc, GetX() - a.GetTopX(), GetY() - a.GetTopY());

  visited++;
}

bool
MathMLStringNode::HasDecimalPoint() const
{
  return false;
}

scaled
MathMLStringNode::GetDecimalPointEdge() const
{
  return GetX();
}

unsigned
MathMLStringNode::GetLogicalContentLength() const
{
  return content.length();
}

String*
MathMLStringNode::GetRawContent() const
{
  return allocString(content);
}
