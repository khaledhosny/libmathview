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

#include "Adaptors.hh"
#include "ChildList.hh"
#include "FormattingContext.hh"
#include "Globals.hh"
#include "MathMLDummyElement.hh"
#include "MathMLFormattingEngineFactory.hh"
#include "MathMLMultiScriptsElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLView.hh"
#include "for_each_if.h"

MathMLMultiScriptsElement::MathMLMultiScriptsElement(const SmartPtr<class MathMLView>& view)
  : MathMLContainerElement(view)
{
}

MathMLMultiScriptsElement::~MathMLMultiScriptsElement()
{
}

void
MathMLMultiScriptsElement::SetBase(const SmartPtr<MathMLElement>& elem)
{
  if (elem != base)
    {
      if (elem) elem->setParent(this);
      base = elem;
      setDirtyLayout();
    }
}

void
MathMLMultiScriptsElement::SetScriptsSize(unsigned size)
{
  assert(size <= subScript.size());
  if (size != subScript.size())
    {
      assert(subScript.size() == superScript.size());
      for (unsigned i = size; i < subScript.size(); i++)
	{
	  SetSubScript(i, 0);
	  SetSuperScript(i, 0);
	}
      subScript.resize(size);
      superScript.resize(size);
      setDirtyLayout();
    }
}

void
MathMLMultiScriptsElement::SetPreScriptsSize(unsigned size)
{
  assert(size <= preSubScript.size());
  if (size != preSubScript.size())
    {
      assert(preSubScript.size() == preSuperScript.size());
      for (unsigned i = size; i < preSubScript.size(); i++)
	{
	  SetPreSubScript(i, 0);
	  SetPreSuperScript(i, 0);
	}
      preSubScript.resize(size);
      preSuperScript.resize(size);
      setDirtyLayout();
    }
}

SmartPtr<MathMLElement>
MathMLMultiScriptsElement::GetSubScript(unsigned i) const
{
  assert(i < subScript.size());
  return subScript[i];
}

SmartPtr<MathMLElement>
MathMLMultiScriptsElement::GetSuperScript(unsigned i) const
{
  assert(i < superScript.size());
  return superScript[i];
}

void
MathMLMultiScriptsElement::SetSubScript(unsigned i, const SmartPtr<MathMLElement>& elem)
{
  assert(i <= subScript.size());
  if (i == subScript.size())
    {
      assert(subScript.size() == superScript.size());
      subScript.push_back(elem);
      superScript.push_back(0);
    }
  else if (elem != subScript[i])
    {
      if (elem) elem->setParent(this);
      subScript[i] = elem;
      setDirtyLayout();
    }
}

void
MathMLMultiScriptsElement::SetSuperScript(unsigned i, const SmartPtr<MathMLElement>& elem)
{
  assert(i <= superScript.size());
  if (i == superScript.size())
    {
      assert(superScript.size() == subScript.size());
      subScript.push_back(0);
      superScript.push_back(elem);
    }
  else if (elem != superScript[i])
    {
      if (elem) elem->setParent(this);
      superScript[i] = elem;
      setDirtyLayout();
    }
}

SmartPtr<MathMLElement>
MathMLMultiScriptsElement::GetPreSubScript(unsigned i) const
{
  assert(i < preSubScript.size());
  return preSubScript[i];
}

SmartPtr<MathMLElement>
MathMLMultiScriptsElement::GetPreSuperScript(unsigned i) const
{
  assert(i < preSuperScript.size());
  return preSuperScript[i];
}

void
MathMLMultiScriptsElement::SetPreSubScript(unsigned i, const SmartPtr<MathMLElement>& elem)
{
  assert(i <= preSubScript.size());
  if (i == preSubScript.size())
    {
      assert(preSubScript.size() == preSuperScript.size());
      preSubScript.push_back(elem);
      preSuperScript.push_back(0);
    }
  else if (elem != preSubScript[i])
    {
      if (elem) elem->setParent(this);
      preSubScript[i] = elem;
      setDirtyLayout();
    }
}

void
MathMLMultiScriptsElement::SetPreSuperScript(unsigned i, const SmartPtr<MathMLElement>& elem)
{
  assert(i <= preSuperScript.size());
  if (i == preSuperScript.size())
    {
      assert(preSubScript.size() == preSuperScript.size());
      preSubScript.push_back(0);
      preSuperScript.push_back(elem);
    }
  else if (elem != preSuperScript[i])
    {
      if (elem) elem->setParent(this);
      preSuperScript[i] = elem;
      setDirtyLayout();
    }
}

#if 0
void
MathMLMultiScriptsElement::Replace(const SmartPtr<MathMLElement>& oldElem, const SmartPtr<MathMLElement>& newElem)
{
  assert(oldElem);
  if (oldElem == base) SetBase(newElem);
  std::vector< SmartPtr<MathMLElement> >::iterator p;

  if ((p = std::find(subScript.begin(), subScript.end(), oldElem)) != subScript.end())
    SetSubScript(p - subScript.begin(), newElem);
  else if ((p = std::find(superScript.begin(), superScript.end(), oldElem)) != superScript.end())
    SetSuperScript(p - superScript.begin(), newElem);
  else if ((p = std::find(preSubScript.begin(), preSubScript.end(), oldElem)) != preSubScript.end())
    SetPreSubScript(p - preSubScript.begin(), newElem);
  else if ((p = std::find(preSuperScript.begin(), preSuperScript.end(), oldElem)) != preSuperScript.end())
    SetPreSuperScript(p - preSuperScript.begin(), newElem);
}
#endif

void
MathMLMultiScriptsElement::construct()
{
  if (dirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      ChildList children(getDOMElement(), MATHML_NS_URI, "*");
      unsigned i = 0;
      unsigned nScripts = 0;
      unsigned nPreScripts = 0;
      unsigned n = children.get_length();
      bool preScripts = false;

      while (i < n)
	{
	  DOM::Node node = children.item(i);

	  if (i == 0)
	    {
	      SmartPtr<MathMLElement> elem;
	      if (nodeLocalName(node) != "none" && nodeLocalName(node) != "mprescripts")
		{
		  elem = getFormattingNode(node);
		  i++;
		}
	      if (elem) SetBase(elem);
	      else if (!is_a<MathMLDummyElement>(base)) SetBase(getFactory()->createDummyElement(getView()));
	    }
	  else if (nodeLocalName(node) == "mprescripts")
	    {
	      if (preScripts)
		Globals::logger(LOG_WARNING, "multiple <mprescripts> elements in mmultiscript");
	      else
		preScripts = true;
	      i++;
	    }
	  else if (!preScripts)
	    {
	      SmartPtr<MathMLElement> sub;
	      SmartPtr<MathMLElement> sup;

	      if (nodeLocalName(node) != "none")
		sub = getFormattingNode(node);
	      i++;
	      
	      if (i < n)
		{
		  node = children.item(i);
		  if (nodeLocalName(node) != "none" && nodeLocalName(node) != "mprescripts")
		    sup = getFormattingNode(node);
		  if (nodeLocalName(node) != "mprescripts") i++;
		}

	      if (sub || sup)
		{
		  SetSubScript(nScripts, sub);
		  SetSuperScript(nScripts, sup);
		  nScripts++;
		}
	    }
	  else
	    {
	      SmartPtr<MathMLElement> sub;
	      SmartPtr<MathMLElement> sup;

	      if (nodeLocalName(node) != "none")
		sub = getFormattingNode(node);
	      i++;
	      
	      if (i < n)
		{
		  node = children.item(i);
		  if (nodeLocalName(node) != "none" && nodeLocalName(node) != "mprescripts")
		    sup = getFormattingNode(node);
		  if (nodeLocalName(node) != "mprescripts") i++;
		}

	      if (sub || sup)
		{
		  SetPreSubScript(nPreScripts, sub);
		  SetPreSuperScript(nPreScripts, sup);
		  nPreScripts++;
		}
	    }
	}
      
      if (n == 0 && !is_a<MathMLDummyElement>(base)) base = getFactory()->createDummyElement(getView());
      SetScriptsSize(nScripts);
      SetPreScriptsSize(nPreScripts);
#endif // HAVE_GMETADOM

      if (base) base->construct();
      for_each_if(subScript.begin(), subScript.end(), NotNullPredicate(), ConstructAdaptor());
      for_each_if(superScript.begin(), superScript.end(), NotNullPredicate(), ConstructAdaptor());
      for_each_if(preSubScript.begin(), preSubScript.end(), NotNullPredicate(), ConstructAdaptor());
      for_each_if(preSuperScript.begin(), preSuperScript.end(), NotNullPredicate(), ConstructAdaptor());

      resetDirtyStructure();
    }
}

#if 0
void
MathMLMultiScriptsElement::Setup(RenderingEnvironment& env)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      background = env.GetBackgroundColor();

      assert(base);
      base->Setup(env);

      env.Push();
      env.AddScriptLevel(1);
      env.SetDisplayStyle(false);

      for_each_if(subScript.begin(), subScript.end(), NotNullPredicate(), std::bind2nd(SetupAdaptor(), &env));
      for_each_if(superScript.begin(), superScript.end(), NotNullPredicate(), std::bind2nd(SetupAdaptor(), &env));
      for_each_if(preSubScript.begin(), preSubScript.end(), NotNullPredicate(), std::bind2nd(SetupAdaptor(), &env));
      for_each_if(preSuperScript.begin(), preSuperScript.end(), NotNullPredicate(), std::bind2nd(SetupAdaptor(), &env));

      ScriptSetup(env);
      env.Drop();

      resetDirtyAttribute();
    }
}

void
MathMLMultiScriptsElement::DoLayout(const class FormattingContext& ctxt)
{
  if (dirtyLayout(ctxt))
    {
      assert(base);
      base->DoLayout(ctxt);

      BoundingBox subScriptBox;
      BoundingBox superScriptBox;
      scaled scriptsWidth = 0;
      scaled preScriptsWidth = 0;

      std::vector< SmartPtr<MathMLElement> >::const_iterator pSub;
      std::vector< SmartPtr<MathMLElement> >::const_iterator pSup;

      for (pSub = subScript.begin(), pSup = superScript.begin();
	   pSub != subScript.end();
	   pSub++, pSup++)
	{
	  assert(pSup != superScript.end());
	  BoundingBox subBox;
	  BoundingBox supBox;

	  if (*pSub)
	    {
	      (*pSub)->DoLayout(ctxt);
	      subBox = (*pSub)->GetBoundingBox();
	    }
	  if (*pSup)
	    {
	      (*pSup)->DoLayout(ctxt);
	      supBox = (*pSup)->GetBoundingBox();
	    }

	  subScriptBox.append(subBox);
	  superScriptBox.append(supBox);
	  scriptsWidth = std::max(scriptsWidth, std::max(subBox.horizontalExtent(), supBox.horizontalExtent()));
	}

      for (pSub = preSubScript.begin(), pSup = preSuperScript.begin();
	   pSub != preSubScript.end();
	   pSub++, pSup++)
	{
	  assert(pSup != preSuperScript.end());
	  BoundingBox subBox;
	  BoundingBox supBox;

	  if (*pSub)
	    {
	      (*pSub)->DoLayout(ctxt);
	      subBox = (*pSub)->GetBoundingBox();
	    }
	  if (*pSup)
	    {
	      (*pSup)->DoLayout(ctxt);
	      supBox = (*pSup)->GetBoundingBox();
	    }

	  subScriptBox.append(subBox);
	  superScriptBox.append(supBox);
	  preScriptsWidth = std::max(preScriptsWidth, std::max(subBox.horizontalExtent(), supBox.horizontalExtent()));
	}

      DoScriptLayout(base->GetBoundingBox(), subScriptBox, superScriptBox,
		     subShiftX, subShiftY, superShiftX, superShiftY);
  
      box = base->GetBoundingBox();
      box.width += preScriptsWidth + scriptsWidth;

      if (subScriptBox.defined())
	{
	  box.height  = std::max(box.height, subScriptBox.height - subShiftY);
	  box.depth = std::max(box.depth, subScriptBox.depth + subShiftY);
	}

      if (superScriptBox.defined())
	{
	  box.height  = std::max(box.height, superScriptBox.height + superShiftY);
	  box.depth = std::max(box.depth, superScriptBox.depth - superShiftY);
	}

      DoEmbellishmentLayout(this, box);

      resetDirtyLayout(ctxt);
    }
}

void
MathMLMultiScriptsElement::SetPosition(const scaled& x0, const scaled& y0)
{
  scaled x = x0;
  scaled y = y0;

  position.x = x;
  position.y = y;

  SetEmbellishmentPosition(this, x, y);

  std::vector< SmartPtr<MathMLElement> >::reverse_iterator preSub;
  std::vector< SmartPtr<MathMLElement> >::reverse_iterator preSup;

  for (preSub = preSubScript.rbegin(), preSup = preSuperScript.rbegin();
       preSub != preSubScript.rend();
       preSub++, preSup++)
    {
      assert(preSup != preSuperScript.rend());
      scaled scriptW = 0;
      if (*preSub) 
	{
	  (*preSub)->SetPosition(x, y + subShiftY);
	  scriptW = (*preSub)->GetBoundingBox().horizontalExtent();
	}
      if (*preSup)
	{
	  (*preSup)->SetPosition(x, y - superShiftY);
	  scriptW = std::max(scriptW, (*preSup)->GetBoundingBox().horizontalExtent());
	}

      x += scriptW;
    }

  assert(base);
  base->SetPosition(x, y);

  std::vector< SmartPtr<MathMLElement> >::iterator pSub;
  std::vector< SmartPtr<MathMLElement> >::iterator pSup;

  for (pSub = subScript.begin(), pSup = superScript.begin();
       pSub != subScript.end();
       pSub++, pSup++)
    {
      assert(pSup != superScript.end());
      scaled scriptW = 0;
      if (*pSub) 
	{
	  (*pSub)->SetPosition(x + subShiftX, y + subShiftY);
	  scriptW = (*pSub)->GetBoundingBox().horizontalExtent();
	}
      if (*pSup)
	{
	  (*pSup)->SetPosition(x + superShiftX, y - superShiftY);
	  scriptW = std::max(scriptW, (*pSup)->GetBoundingBox().horizontalExtent());
	}

      x += scriptW;
    }
}

void
MathMLMultiScriptsElement::Render(const DrawingArea& area)
{
  if (Exposed(area))
    {
      RenderBackground(area);
      assert(base);
      base->Render(area);
      for_each_if(subScript.begin(), subScript.end(), NotNullPredicate(), std::bind2nd(RenderAdaptor(), &area));
      for_each_if(superScript.begin(), superScript.end(), NotNullPredicate(), std::bind2nd(RenderAdaptor(), &area));
      for_each_if(preSubScript.begin(), preSubScript.end(), NotNullPredicate(), std::bind2nd(RenderAdaptor(), &area));
      for_each_if(preSuperScript.begin(), preSuperScript.end(), NotNullPredicate(), std::bind2nd(RenderAdaptor(), &area));
      ResetDirty();
    }
}

void
MathMLMultiScriptsElement::ReleaseGCs()
{
  MathMLElement::ReleaseGCs();
  assert(base);
  base->ReleaseGCs();
  for_each_if(subScript.begin(), subScript.end(), NotNullPredicate(), ReleaseGCsAdaptor());
  for_each_if(superScript.begin(), superScript.end(), NotNullPredicate(), ReleaseGCsAdaptor());
  for_each_if(preSubScript.begin(), preSubScript.end(), NotNullPredicate(), ReleaseGCsAdaptor());
  for_each_if(preSuperScript.begin(), preSuperScript.end(), NotNullPredicate(), ReleaseGCsAdaptor());
}

SmartPtr<MathMLElement>
MathMLMultiScriptsElement::Inside(const scaled& x, const scaled& y)
{
  if (IsInside(x, y))
    {
      assert(base);
      if (SmartPtr<MathMLElement> inside = base->Inside(x, y)) return inside;

      for (std::vector< SmartPtr<MathMLElement> >::iterator elem = preSubScript.begin();
	   elem != preSubScript.end(); elem++)
	if (*elem)
	  {
	    SmartPtr<MathMLElement> inside = (*elem)->Inside(x, y);
	    if (inside) return inside;
	  }

      for (std::vector< SmartPtr<MathMLElement> >::iterator elem = preSuperScript.begin();
	   elem != preSuperScript.end(); elem++)
	if (*elem)
	  {
	    SmartPtr<MathMLElement> inside = (*elem)->Inside(x, y);
	    if (inside) return inside;
	  }

      for (std::vector< SmartPtr<MathMLElement> >::iterator elem = subScript.begin();
	   elem != subScript.end(); elem++)
	if (*elem)
	  {
	    SmartPtr<MathMLElement> inside = (*elem)->Inside(x, y);
	    if (inside) return inside;
	  }

      for (std::vector< SmartPtr<MathMLElement> >::iterator elem = superScript.begin();
	   elem != superScript.end(); elem++)
	if (*elem)
	  {
	    SmartPtr<MathMLElement> inside = (*elem)->Inside(x, y);
	    if (inside) return inside;
	  }

      return this;
    }
  else
    return 0;
}

scaled
MathMLMultiScriptsElement::GetLeftEdge() const
{
  if (preSubScript.size() > 0)
    {
      assert(preSuperScript.size() == preSubScript.size());
      SmartPtr<MathMLElement> sub = GetPreSubScript(preSubScript.size() - 1);
      SmartPtr<MathMLElement> sup = GetPreSuperScript(preSuperScript.size() - 1);
      if (sub && sup)
	return std::min(sub->GetLeftEdge(), sup->GetLeftEdge());
      else if (sub)
	return sub->GetLeftEdge();
      else if (sup)
	return sup->GetLeftEdge();
      else
	assert(0);
    }
  else
    {
      assert(base);
      return base->GetLeftEdge();
    }
}

scaled
MathMLMultiScriptsElement::GetRightEdge() const
{
  if (subScript.size() > 0)
    {
      assert(subScript.size() == superScript.size());
      SmartPtr<MathMLElement> sub = GetSubScript(subScript.size() - 1);
      SmartPtr<MathMLElement> sup = GetSuperScript(superScript.size() - 1);
      if (sub && sup)
	return std::min(sub->GetLeftEdge(), sup->GetLeftEdge());
      else if (sub)
	return sub->GetLeftEdge();
      else if (sup)
	return sup->GetLeftEdge();
      else
	assert(0);      
    }
  else
    {
      assert(base);
      return base->GetRightEdge();
    }
}
#endif

SmartPtr<MathMLOperatorElement>
MathMLMultiScriptsElement::GetCoreOperator()
{
  if (base) return base->GetCoreOperator();
  else return 0;
}

void
MathMLMultiScriptsElement::setFlagDown(Flags f)
{
  MathMLElement::setFlagDown(f);
  if (base) base->setFlagDown(f);
  for_each_if(subScript.begin(), subScript.end(), NotNullPredicate(), std::bind2nd(SetFlagDownAdaptor(), f));
  for_each_if(superScript.begin(), superScript.end(), NotNullPredicate(), std::bind2nd(SetFlagDownAdaptor(), f));
  for_each_if(preSubScript.begin(), preSubScript.end(), NotNullPredicate(), std::bind2nd(SetFlagDownAdaptor(), f));
  for_each_if(preSuperScript.begin(), preSuperScript.end(), NotNullPredicate(), std::bind2nd(SetFlagDownAdaptor(), f));
}

void
MathMLMultiScriptsElement::resetFlagDown(Flags f)
{
  MathMLElement::resetFlagDown(f);
  if (base) base->resetFlagDown(f);
  for_each_if(subScript.begin(), subScript.end(), NotNullPredicate(), std::bind2nd(ResetFlagDownAdaptor(), f));
  for_each_if(superScript.begin(), superScript.end(), NotNullPredicate(), std::bind2nd(ResetFlagDownAdaptor(), f));
  for_each_if(preSubScript.begin(), preSubScript.end(), NotNullPredicate(), std::bind2nd(ResetFlagDownAdaptor(), f));
  for_each_if(preSuperScript.begin(), preSuperScript.end(), NotNullPredicate(), std::bind2nd(ResetFlagDownAdaptor(), f));
}

