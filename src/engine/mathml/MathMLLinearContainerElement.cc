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

#include <cassert>

#include <algorithm>
#include <functional>

#include "defs.h"
#include "for_each_if.h"
#include "Adaptors.hh"
#include "ChildList.hh"
#include "MathMLLinearContainerElement.hh"

MathMLLinearContainerElement::MathMLLinearContainerElement(const SmartPtr<MathMLView>& view)
  : MathMLContainerElement(view)
{
}

MathMLLinearContainerElement::~MathMLLinearContainerElement()
{
}

#if 0
void
MathMLLinearContainerElement::construct(AbstractConstructionContext& ctxt)
{
  if (dirtyStructure())
    {
      unsigned i = 0;
      while (SmartPtr<Element> elem = ctxt.getElement(getChild(i), false))
	setChild(i++, elem);
      setSize(i);
      resetDirtyStructure();
    }
}
#endif

void
MathMLLinearContainerElement::construct()
{
  if (dirtyStructure())
    {
      // editing is supported with DOM only
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  ChildList children(getDOMElement(), MATHML_NS_URI, "*");
	  unsigned n = children.get_length();

	  std::vector< SmartPtr<MathMLElement> > newContent;
	  newContent.reserve(n);
	  for (unsigned i = 0; i < n; i++)
	    {
	      DOM::Node node = children.item(i);
	      assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);

	      if (SmartPtr<MathMLElement> elem = getFormattingNode(node))
		newContent.push_back(elem);
	      else
		{
		  // it might be that we get a NULL. In that case it would probably make
		  // sense to create a dummy element, because we filtered MathML
		  // elements only
		}
	    }
	  SwapChildren(newContent);
	}
#endif // HAVE_GMETADOM
      
      // it is better to normalize elements only after all the rendering
      // interfaces have been collected, because the structure might change
      // depending on the actual number of children
      for_each_if(content.begin(), content.end(), NotNullPredicate(), ConstructAdaptor());
      resetDirtyStructure();
    }
}

void
MathMLLinearContainerElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      for_each_if(content.begin(), content.end(), NotNullPredicate(), std::bind2nd(RefineAdaptor(), &context));
      MathMLContainerElement::refine(context);
    }
}

#if 0
void
MathMLLinearContainerElement::Setup(RenderingEnvironment& env)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      background = env.GetBackgroundColor();
      for_each_if(content.begin(), content.end(), NotNullPredicate(), std::bind2nd(SetupAdaptor(), &env));
      resetDirtyAttribute();
    }
}

void
MathMLLinearContainerElement::DoLayout(const FormattingContext& ctxt)
{
  if (dirtyLayout(ctxt))
    {
      for_each_if(content.begin(), content.end(), NotNullPredicate(), std::bind2nd(DoLayoutAdaptor(), &ctxt));
      resetDirtyLayout(ctxt);
    }
}

void
MathMLLinearContainerElement::Render(const DrawingArea& area)
{
  if (Exposed(area))
    {
      RenderBackground(area);
      for_each_if(content.begin(), content.end(), NotNullPredicate(), std::bind2nd(RenderAdaptor(), &area));
      ResetDirty();
    }
}

SmartPtr<MathMLElement>
MathMLLinearContainerElement::Inside(const scaled& x, const scaled& y)
{
  if (IsInside(x, y)) 
    {
      for (std::vector< SmartPtr<MathMLElement> >::iterator elem = content.begin();
	   elem != content.end(); elem++)
	{
	  SmartPtr<MathMLElement> inside = (*elem)->Inside(x, y);
	  if (inside) return inside;
	}
      
      return this;
    }
  else
    return 0;
}

void
MathMLLinearContainerElement::ReleaseGCs()
{
  MathMLElement::ReleaseGCs();
  for_each_if(content.begin(), content.end(), NotNullPredicate(), ReleaseGCsAdaptor());
}
#endif

void
MathMLLinearContainerElement::SetSize(unsigned size)
{
  assert(size <= content.size());
  if (size != content.size())
    {
      for (unsigned i = size; i < content.size(); i++) SetChild(i, 0);
      content.resize(size);
      setDirtyLayout();
    }
}

SmartPtr<MathMLElement>
MathMLLinearContainerElement::GetChild(unsigned i) const
{
  return (i < GetSize()) ? content[i] : 0;
}

void
MathMLLinearContainerElement::SetChild(unsigned i, const SmartPtr<MathMLElement>& elem)
{
  assert(i <= GetSize());

  if (i == GetSize()) Append(elem);
  else if (content[i] != elem)
    {
      if (elem) elem->setParent(this);
      content[i] = elem;
      setDirtyLayout();
    }
}

void
MathMLLinearContainerElement::Append(const SmartPtr<MathMLElement>& elem)
{
  assert(elem);
  elem->setParent(this);
  content.push_back(elem);
  setDirtyLayout();
}

#if 0
void
MathMLLinearContainerElement::Replace(const SmartPtr<MathMLElement>& oldElem,
				      const SmartPtr<MathMLElement>& newElem)
{
  assert(oldElem);
  std::vector< SmartPtr<MathMLElement> >::iterator old = find(content.begin(), content.end(), oldElem);
  assert(old != content.end());
  SetChild(old - content.begin(), newElem);
}
#endif

void
MathMLLinearContainerElement::SwapChildren(std::vector< SmartPtr<MathMLElement> >& newContent)
{
  if (newContent != content)
    {
#if 0
      // reset parent should be done first because the same elements
      // may be present in the following loop as well
      for (std::vector< SmartPtr<MathMLElement> >::iterator p = content.begin();
	   p != content.end();
	   p++)
	if (*p) (*p)->Unlink();
#endif

      for (std::vector< SmartPtr<MathMLElement> >::iterator p = newContent.begin();
	   p != newContent.end();
	   p++)
	if (*p) (*p)->setParent(this);

      content.swap(newContent);
      setDirtyLayout();
    }
}

#if 0
scaled
MathMLLinearContainerElement::GetLeftEdge() const
{
  scaled edge = 0;
  
  for (std::vector< SmartPtr<MathMLElement> >::const_iterator elem = content.begin();
       elem != content.end();
       elem++)
    {
      if (elem == content.begin()) edge = (*elem)->GetLeftEdge();
      else edge = std::min(edge, (*elem)->GetX() + (*elem)->GetLeftEdge());
    }

  return edge;
}

scaled
MathMLLinearContainerElement::GetRightEdge() const
{
  scaled edge = 0;

  for (std::vector< SmartPtr<MathMLElement> >::const_iterator elem = content.begin();
       elem != content.end();
       elem++)
    {
      if (elem == content.begin()) edge = (*elem)->GetRightEdge();
      else edge = std::max(edge, (*elem)->GetX() + (*elem)->GetRightEdge());
    }

  return edge;
}

void
MathMLLinearContainerElement::SetDirty(const Rectangle* rect)
{
  if (!rect || GetRectangle().Overlaps(*rect))
    {
      SetFlag(FDirty);
      for_each_if(content.begin(), content.end(), NotNullPredicate(), std::bind2nd(SetDirtyAdaptor(), rect));
    }
}
#endif

void
MathMLLinearContainerElement::setFlagDown(Flags f)
{
  MathMLElement::setFlagDown(f);
  for_each_if(content.begin(), content.end(), NotNullPredicate(), std::bind2nd(SetFlagDownAdaptor(), f));
}

void
MathMLLinearContainerElement::resetFlagDown(Flags f)
{
  MathMLElement::resetFlagDown(f);
  for_each_if(content.begin(), content.end(), NotNullPredicate(), std::bind2nd(ResetFlagDownAdaptor(), f));
}
