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

#include "Globals.hh"
#include "MathMLCharNode.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLEmbellishedOperatorElement.hh"
#include "FormattingContext.hh"

MathMLEmbellishedOperatorElement::
MathMLEmbellishedOperatorElement(const Ptr<MathMLOperatorElement>& op)
  : coreOp(op)
{
  assert(coreOp);
  script = false;
}

MathMLEmbellishedOperatorElement::~MathMLEmbellishedOperatorElement()
{
}

void
MathMLEmbellishedOperatorElement::Normalize(const Ptr<class MathMLDocument>& doc)
{
#if 0
  if (DirtyStructure())
    {
#endif
      assert(GetChild());
      assert(coreOp);
      coreOp->SetDirtyStructure();

      Ptr<MathMLElement> p = GetParent();
      assert(p);

      Ptr<MathMLContainerElement> pContainer = smart_cast<MathMLContainerElement>(p);
      assert(pContainer);
      cout << "removing embellishment " << endl; 

      Ptr<MathMLElement> oldChild = GetChild();
      SetChild(0);
      pContainer->Replace(this, oldChild);
      oldChild->SetDirtyStructure();
      oldChild->SetDirtyAttributeD();
      oldChild->Normalize(doc);

#if defined(HAVE_GMETADOM)
      doc->setFormattingNode(oldChild->GetDOMElement(), oldChild);
#endif

      ResetDirtyStructure();
#if 0
    }
#endif
}

void
MathMLEmbellishedOperatorElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      script = env.GetScriptLevel() > 0;
      MathMLBinContainerElement::Setup(env);
    }
}

void
MathMLEmbellishedOperatorElement::DoLayout(const class FormattingContext& ctxt)
{
  cout << "embellishment layout " << this << " dirty " << DirtyLayout(ctxt) << endl;

  if (DirtyLayout(ctxt))
    {
      assert(child);
      assert(coreOp);

      scaled totalPadding = script ? 0 : coreOp->GetLeftPadding() + coreOp->GetRightPadding();

      //Globals::logger(LOG_DEBUG, "layout of embellishment %p script %d padding %d", this, script, sp2ipx(totalPadding));

      cout << "child " << static_cast<MathMLElement*>(child) << " dirty layout " << child->DirtyLayout() << endl;

      child->DoLayout(ctxt);
      box = child->GetBoundingBox();

      // WARNING: maybe in this case we should ask for the LAST char node...
      Ptr<const MathMLCharNode> node = coreOp->GetCharNode();
      if (node && isIntegral(node->GetChar())) {
	// WARNING
	// the following patch is needed in order to have integral sign working
	box.width = scaledMax(box.width, box.rBearing);
      }
      box.width += totalPadding;

#ifdef ENABLE_EXTENSIONS
      box.ascent += coreOp->GetTopPadding();
      box.descent += coreOp->GetBottomPadding();
#endif // ENABLE_EXTENSIONS

      ResetDirtyLayout(ctxt);
    }
}

void
MathMLEmbellishedOperatorElement::SetPosition(scaled x, scaled y)
{
  assert(coreOp);
  assert(child);

  position.x = x;
  position.y = y;

#ifdef ENABLE_EXTENSIONS
  child->SetPosition(x + (script ? 0 : coreOp->GetLeftPadding()), y + coreOp->GetTopPadding());
#else
  child->SetPosition(x + (script ? 0 : coreOp->GetLeftPadding()), y);
#endif // ENABLE_EXTENSIONS
}

Ptr<MathMLCharNode>
MathMLEmbellishedOperatorElement::GetCharNode() const
{
  if (!coreOp || child != coreOp)
    return 0;
  else
    return coreOp->GetCharNode();
}

void
MathMLEmbellishedOperatorElement::Lift(const Ptr<MathMLDocument>& doc)
{
  assert(GetChild());

  Ptr<MathMLContainerElement> parent = smart_cast<MathMLContainerElement>(GetParent());
  assert(parent);

  Ptr<MathMLContainerElement> grandParent = smart_cast<MathMLContainerElement>(parent->GetParent());
  assert(grandParent);

  cout << "lifting " << this << " from " << static_cast<MathMLContainerElement*>(parent) << " to " 
       << static_cast<MathMLContainerElement*>(grandParent) << endl;

  parent->Replace(this, GetChild());
  grandParent->Replace(parent, this);

#if defined(HAVE_GMETADOM)
  doc->setFormattingNode(GetChild()->GetDOMElement(), GetChild());
  doc->setFormattingNode(parent->GetDOMElement(), this);
#endif
}
