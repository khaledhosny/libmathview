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

#include "CharMapper.hh"
#include "MathMLDummyElement.hh"
#include "RenderingEnvironment.hh"

MathMLDummyElement::MathMLDummyElement(GdomeElement* node) : 
  MathMLElement(node, TAG_DUMMY)
{
}

MathMLDummyElement::~MathMLDummyElement()
{
}

void
MathMLDummyElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  FontifiedChar fChar;

  env->charMapper.FontifyChar(fChar, env->GetFontAttributes(), 'a');
  assert(fChar.font != NULL);
  
  fChar.font->GetBoundingBox(box);
}

void
MathMLDummyElement::DoBoxedLayout(LayoutId id, BreakId, scaled)
{
  ConfirmLayout(id);
  ResetDirtyLayout(id);
}

void
MathMLDummyElement::Render(const DrawingArea& area)
{
  if (!HasDirtyChildren()) return;

  RenderBackground(area);

  if (fGC[IsSelected()] == NULL) {
    GraphicsContextValues values;
    values.foreground = IsSelected() ? area.GetSelectionForeground() : RED_COLOR;
    values.lineStyle = LINE_STYLE_SOLID;
    fGC[IsSelected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_LINE_STYLE);
  }

  area.DrawRectangle(fGC[IsSelected()], GetShape());

  ResetDirty();
}
