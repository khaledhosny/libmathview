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

#include "MathMLMarkNode.hh"
#include "AttributeSignature.hh"
#include "RenderingEnvironment.hh"
#include "ValueConversion.hh"

MathMLMarkNode::MathMLMarkNode(MarkAlignType e)
{
  edge = e;
}

MathMLMarkNode::~MathMLMarkNode()
{
}

void
MathMLMarkNode::Setup(RenderingEnvironment& env)
{
  // next a tricky operation. A token can contain a <malignmark>
  // element which has an attribute (edge) possibily inherited from
  // the rendering environment.
  // This is the price for having a more natural token element with just
  // text nodes as content... otherwise we would have frames.
  static AttributeSignature sig = { ATTR_EDGE, alignMarkEdgeParser, "left", NULL };

  if (SmartPtr<MathMLAttribute> attribute = env.GetAttribute(ATTR_EDGE))
    // ok, we have to do something only in case the attribute edge
    // wasn't explicitly set inside the mark (see Parser.cc)
    if (edge == MARK_ALIGN_NOTVALID)
      if (SmartPtr<Value> value = attribute->getParsedValue(&sig))
	edge = ToMarkAlignId(value);

  // since left is the default value for the edge attribute,
  // we set to left even in case the value is wrong
  if (edge == MARK_ALIGN_NOTVALID) edge = MARK_ALIGN_LEFT;
}

void
MathMLMarkNode::DoLayout(const FormattingContext&)
{
  box.unset();
}

void
MathMLMarkNode::Render(const DrawingArea&)
{
}
