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

#include "Layout.hh"
#include "MathMLCharNode.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLEmbellishedOperatorElement.hh"

MathMLEmbellishedOperatorElement::
MathMLEmbellishedOperatorElement(MathMLOperatorElement* op) // the core operator
  : MathMLContainerElement(NULL, TAG_MO)
{
  assert(op != NULL);
  coreOp = op;
  script = false;
}

MathMLEmbellishedOperatorElement::~MathMLEmbellishedOperatorElement()
{
}

void
MathMLEmbellishedOperatorElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);
  script = env->GetScriptLevel() > 0;
  MathMLContainerElement::Setup(env);
}

void
MathMLEmbellishedOperatorElement::DoBoxedLayout(LayoutId id, BreakId, scaled availWidth)
{
  if (!HasDirtyLayout(id, availWidth)) return;

  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);
  assert(coreOp != NULL);

  scaled totalPadding = script ? 0 : coreOp->GetLeftPadding() + coreOp->GetRightPadding();

  content.GetFirst()->DoBoxedLayout(id, BREAK_NO, scaledMax(0, availWidth - totalPadding));
  box = content.GetFirst()->GetBoundingBox();

  // WARNING: maybe in this case we should ask for the LAST char node...
  const MathMLCharNode* node = coreOp->GetCharNode();
  if (node != NULL && isIntegral(node->GetChar())) {
    // WARNING
    // the following patch is needed in order to have integral sign working
    box.width = scaledMax(box.width, box.rBearing);
  }
  box.width += totalPadding;

#ifdef ENABLE_EXTENSIONS
  box.ascent += coreOp->GetTopPadding();
  box.tAscent += coreOp->GetTopPadding();
  box.descent += coreOp->GetBottomPadding();
  box.tDescent += coreOp->GetBottomPadding();
#endif // ENABLE_EXTENSIONS

  ConfirmLayout(id);

  ResetDirtyLayout(id, availWidth);
}

#if 0
void
MathMLEmbellishedOperatorElement::DoLayout(LayoutId id, Layout& layout)
{
  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);
  assert(coreOp != NULL);

  layout.Append(coreOp->GetLeftPadding());
  layout.Append(coreOp);
  layout.Append(coreOp->GetRightPadding());
}
#endif

void
MathMLEmbellishedOperatorElement::SetPosition(scaled x, scaled y)
{
  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);
  assert(coreOp != NULL);

  position.x = x;
  position.y = y;

#ifdef ENABLE_EXTENSIONS
  content.GetFirst()->SetPosition(x + (script ? 0 : coreOp->GetLeftPadding()), y + coreOp->GetTopPadding());
#else
  content.GetFirst()->SetPosition(x + (script ? 0 : coreOp->GetLeftPadding()), y);
#endif // ENABLE_EXTENSIONS
}

bool
MathMLEmbellishedOperatorElement::IsEmbellishedOperator() const
{
  return true;
}

const MathMLCharNode*
MathMLEmbellishedOperatorElement::GetCharNode() const
{
  if (content.GetSize() != 1 || coreOp == NULL || content.GetFirst() != coreOp)
    return NULL;

  return coreOp->GetCharNode();
}
