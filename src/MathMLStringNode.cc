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
#include "Gtk_DrawingArea.hh"
#include "ShaperManager.hh"
#include "Gtk_RenderingContext.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"

unsigned MathMLStringNode::visited = 0;

MathMLStringNode::MathMLStringNode(const String& c)
  : content(c)
{
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
}

void
MathMLStringNode::DoLayout(const FormattingContext& ctxt)
{
#if 0
  if (!area) area = env.shaperManager->shape(ctxt, toUCS4String(content));
  assert(area);
  box = area->box();
#endif
}

AreaRef
MathMLStringNode::format(MathFormattingContext& ctxt)
{
  return ctxt.getDevice().string(ctxt, content);
}

void
MathMLStringNode::Render(const DrawingArea& a)
{
#if 0
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
#endif
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

String
MathMLStringNode::GetRawContent() const
{
  return content;
}
