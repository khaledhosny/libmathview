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
#include <stddef.h>

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "Iterator.hh"
#include "DocumentElement.hh"
#include "RenderingEnvironment.hh"

DocumentElement::DocumentElement()
  : MathMLLinearContainerElement(0)
{
}

#if defined(HAVE_GMETADOM)
DocumentElement::DocumentElement(const DOM::Document& doc)
  : MathMLLinearContainerElement(doc.get_documentElement())
{
}
#endif

DocumentElement::~DocumentElement()
{
}

void
DocumentElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);
  sppm = env->GetScaledPointsPerEm();
  MathMLLinearContainerElement::Setup(env);
}

void
DocumentElement::DoBoxedLayout(LayoutId id, BreakId bid, scaled maxWidth)
{
  if (!HasDirtyLayout(id, maxWidth)) return;

  box.Null();

  for (Iterator< Ptr<MathMLElement> > elem(content); elem.More(); elem.Next())
    {
      assert(elem() != 0);

      elem()->DoBoxedLayout(id, bid, maxWidth);
      const BoundingBox& elemBox = elem()->GetBoundingBox();
      if (box.IsNull()) box = elemBox;
      else {
	box.width = scaledMax(box.width, elemBox.width);
	box.descent += elemBox.GetHeight() + 2 * sppm;
      }
    }

  ConfirmLayout(id);

  ResetDirtyLayout(id, maxWidth);
}

void
DocumentElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;

  scaled offset = y;

  for (Iterator< Ptr<MathMLElement> > elem(content); elem.More(); elem.Next())
    {
      assert(elem() != 0);

      const BoundingBox& elemBox = elem()->GetBoundingBox();

      if (!elem.IsFirst()) offset += elemBox.ascent;
      elem()->SetPosition(x, offset);
      offset += elemBox.descent + 2 * sppm;
    }
}
