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

#include "Clock.hh"
#include "Globals.hh"
#include "MathMLView.hh"
#include "scaled.hh"
#include "scaledConv.hh"
#include "traverseAux.hh"
#include "MathMLFormattingEngineFactory.hh"
#include "RefinementContext.hh"
#include "RenderingEnvironment.hh"

MathMLView::MathMLView(const SmartPtr<MathMLViewContext>& c)
  : context(c)
{
  freezeCounter = 0;
  defaultFontSize = Globals::configuration.GetFontSize();
  area = 0;
  fontManager = 0;
}

MathMLView::~MathMLView()
{
}

void
MathMLView::Init(class DrawingArea* a, class FontManager* fm)
{
  assert(a != NULL);
  assert(fm != NULL);

  if (root)
    {
      root->ReleaseGCs();
      root->SetDirtyAttributeD();
      root->SetDirtyLayout();
      //root->SetDirty();
    }

  area = a;
  fontManager = fm;
}

bool
MathMLView::freeze()
{
  return freezeCounter++ == 0;
}

bool
MathMLView::thaw()
{
  assert(freezeCounter > 0);
  return --freezeCounter == 0;
}

void
MathMLView::setDefaultFontSize(unsigned size)
{
  assert(size > 0);
  if (defaultFontSize != size)
    {
      defaultFontSize = size;
      if (root)
	{
	  root->SetDirtyAttributeD();
	  root->SetDirtyLayout();	  
	}
    }
}

void
MathMLView::DOMSubtreeModifiedListener::handleEvent(const DOM::Event& ev)
{
  DOM::MutationEvent me(ev);
  assert(me);

  if (SmartPtr<MathMLElement> elem = findMathMLElement(view, DOM::Element(me.get_target())))
    {
      elem->SetDirtyStructure();
      elem->SetDirtyAttributeD();
    }
}

void
MathMLView::DOMAttrModifiedListener::handleEvent(const DOM::Event& ev)
{
  DOM::MutationEvent me(ev);
  assert(me);

  if (SmartPtr<MathMLElement> elem = findMathMLElement(view, DOM::Element(me.get_target())))
    elem->SetDirtyAttribute();
}

void
MathMLView::setRoot(const DOM::Element& elem)
{
  if (root)
    {
      DOM::EventTarget et(root->getDOMElement());
      assert(et);

      et.removeEventListener("DOMNodeInserted", *subtreeModifiedListener, false);
      et.removeEventListener("DOMNodeRemoved", *subtreeModifiedListener, false);
      et.removeEventListener("DOMCharacterDataModified", *subtreeModifiedListener, false);
      et.removeEventListener("DOMAttrModified", *attrModifiedListener, false);

      delete subtreeModifiedListener;
      delete attrModifiedListener;
      subtreeModifiedListener = 0;
      attrModifiedListener = 0;
    }

  if (elem)
    {
      root = getContext()->engineFactory->createElement(this, nodeLocalName(elem));
      root->setDOMElement(elem);

      DOM::EventTarget et(root->getDOMElement());
      assert(et);

      subtreeModifiedListener = new DOMSubtreeModifiedListener(this);
      attrModifiedListener = new DOMAttrModifiedListener(this);

      et.addEventListener("DOMNodeInserted", *subtreeModifiedListener, false);
      et.addEventListener("DOMNodeRemoved", *subtreeModifiedListener, false);
      et.addEventListener("DOMCharacterDataModified", *subtreeModifiedListener, false);
      et.addEventListener("DOMAttrModified", *attrModifiedListener, false);
    }
}

SmartPtr<MathMLElement>
MathMLView::getElementAt(const scaled& x, const scaled& y) const
{
  // WARNING: x and y must be absolute coordinates w.r.t. the drawing
  // area, because at this level we do not known whether the drawing
  // area is scrollable (as in the case of Gtk_DrawingArea) or not
  // (PS_DrawingArea). The caller must properly adjust x and y before
  // calling this method
  return root ? root->Inside(x, y) : 0;
}

BoundingBox
MathMLView::getBoundingBox() const
{
  layout();
  if (root)
    return root->GetBoundingBox();
  else
    return BoundingBox();
}

Rectangle
MathMLView::getRectangle() const
{
  layout();
  if (root)
    return Rectangle(root->GetX(), root->GetY(), root->GetBoundingBox());
  else
    return Rectangle();
}

void
MathMLView::layout() const
{
  if (root && !frozen())
    {
      Clock formatTime;
      formatTime.Start();

      if (root->DirtyStructure())
	{
	  Clock perf;
	  perf.Start();
	  root->construct();
	  perf.Stop();
	  Globals::logger(LOG_INFO, "construction time: %dms", perf());
	}

      if (root->DirtyAttribute() || root->DirtyAttributeP())
	{
	  Clock perf;
	  RefinementContext rc;
	  perf.Start();
	  root->refine(rc);
	  perf.Stop();
	  Globals::logger(LOG_INFO, "refinement time: %dms", perf());
	  UnitValue size(defaultFontSize, UNIT_PT);
	  RenderingEnvironment env(getContext()->areaFactory, getContext()->shaperManager);
	  env.SetFontSize(size);
	  perf.Start();
	  root->Setup(env);
	  perf.Stop();
	  Globals::logger(LOG_INFO, "setup time: %dms", perf());
	}

      if (root->DirtyLayout())
	{
	  Clock perf;
	  perf.Start();
	  root->DoLayout(FormattingContext(LAYOUT_MIN,0));
	  perf.Stop();
	  Globals::logger(LOG_INFO, "minimum layout time: %dms", perf());

	  perf.Start();
	  root->DoLayout(FormattingContext(LAYOUT_AUTO, std::max(scaled(0), area->GetWidth() -  2 * area->GetXMargin())));
	  root->SetPosition(area->GetXMargin(), area->GetYMargin() + root->GetBoundingBox().height);
	  perf.Stop();
	  Globals::logger(LOG_INFO, "layout time: %dms", perf());
	}

      formatTime.Stop();
      Globals::logger(LOG_INFO, "FORMATTING TIME: %dms", formatTime());
    }
}

void
MathMLView::render(const Rectangle* rect) const
{
  assert(area);

  layout();
  //SetDirty(rect);

  if (root)
    {
      Clock perf;
      perf.Start();
      MathMLStringNode::visited = 0;
      root->Render(*area);
      perf.Stop();
      Globals::logger(LOG_INFO, "rendering time: %dms (visited %d)", perf(), MathMLStringNode::visited);
    }

  if (rect) area->Update(*rect);
  else area->Update();
}
