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

#include <algorithm>
#include <functional>

#include <assert.h>
#include <stddef.h>

#include "Globals.hh"
#include "Adaptors.hh"
#include "for_each_if.h"
#include "ChildList.hh"
#include "MathMLDummyElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLMultiScriptsElement.hh"
#include "FormattingContext.hh"

MathMLMultiScriptsElement::MathMLMultiScriptsElement()
{
}

#if defined(HAVE_GMETADOM)
MathMLMultiScriptsElement::MathMLMultiScriptsElement(const DOM::Element& node)
  : MathMLContainerElement(node)
{
}
#endif

MathMLMultiScriptsElement::~MathMLMultiScriptsElement()
{
}

void
MathMLMultiScriptsElement::SetBase(const Ptr<MathMLElement>& elem)
{
  if (elem != base)
    {
      if (base) base->Unlink();
      if (elem) elem->Link(this);
      base = elem;
      SetDirtyLayout();
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
      SetDirtyLayout();
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
      SetDirtyLayout();
    }
}

Ptr<MathMLElement>
MathMLMultiScriptsElement::GetSubScript(unsigned i) const
{
  assert(i < subScript.size());
  return subScript[i];
}

Ptr<MathMLElement>
MathMLMultiScriptsElement::GetSuperScript(unsigned i) const
{
  assert(i < superScript.size());
  return superScript[i];
}

void
MathMLMultiScriptsElement::SetSubScript(unsigned i, const Ptr<MathMLElement>& elem)
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
      if (subScript[i]) subScript[i]->Unlink();
      if (elem) elem->Link(this);
      subScript[i] = elem;
      SetDirtyLayout();
    }
}

void
MathMLMultiScriptsElement::SetSuperScript(unsigned i, const Ptr<MathMLElement>& elem)
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
      if (superScript[i]) superScript[i]->Unlink();
      if (elem) elem->Link(this);
      superScript[i] = elem;
      SetDirtyLayout();
    }
}

Ptr<MathMLElement>
MathMLMultiScriptsElement::GetPreSubScript(unsigned i) const
{
  assert(i < preSubScript.size());
  return preSubScript[i];
}

Ptr<MathMLElement>
MathMLMultiScriptsElement::GetPreSuperScript(unsigned i) const
{
  assert(i < preSuperScript.size());
  return preSuperScript[i];
}

void
MathMLMultiScriptsElement::SetPreSubScript(unsigned i, const Ptr<MathMLElement>& elem)
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
      if (preSubScript[i]) preSubScript[i]->Unlink();
      if (elem) elem->Link(this);
      preSubScript[i] = elem;
      SetDirtyLayout();
    }
}

void
MathMLMultiScriptsElement::SetPreSuperScript(unsigned i, const Ptr<MathMLElement>& elem)
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
      if (preSuperScript[i]) preSuperScript[i]->Unlink();
      if (elem) elem->Link(this);
      preSuperScript[i] = elem;
      SetDirtyLayout();
    }
}

void
MathMLMultiScriptsElement::Replace(const Ptr<MathMLElement>& oldElem, const Ptr<MathMLElement>& newElem)
{
  assert(oldElem);
  if (oldElem == base) SetBase(newElem);
  std::vector< Ptr<MathMLElement> >::iterator p;

  if ((p = std::find(subScript.begin(), subScript.end(), oldElem)) != subScript.end())
    SetSubScript(p - subScript.begin(), newElem);
  else if ((p = std::find(superScript.begin(), superScript.end(), oldElem)) != superScript.end())
    SetSuperScript(p - superScript.begin(), newElem);
  else if ((p = std::find(preSubScript.begin(), preSubScript.end(), oldElem)) != preSubScript.end())
    SetPreSubScript(p - preSubScript.begin(), newElem);
  else if ((p = std::find(preSuperScript.begin(), preSuperScript.end(), oldElem)) != preSuperScript.end())
    SetPreSuperScript(p - preSuperScript.begin(), newElem);
}

void
MathMLMultiScriptsElement::Normalize(const Ptr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      ChildList children(GetDOMElement(), MATHML_NS_URI, "*");
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
	      Ptr<MathMLElement> elem;
	      if (nodeLocalName(node) != "none" && nodeLocalName(node) != "mprescripts")
		{
		  elem = doc->getFormattingNode(node);
		  i++;
		}
	      if (elem) SetBase(elem);
	      else if (!is_a<MathMLDummyElement>(base)) SetBase(MathMLDummyElement::create());
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
	      Ptr<MathMLElement> sub;
	      Ptr<MathMLElement> sup;

	      if (nodeLocalName(node) != "none")
		sub = doc->getFormattingNode(node);
	      i++;
	      
	      if (i < n)
		{
		  node = children.item(i);
		  if (nodeLocalName(node) != "none" && nodeLocalName(node) != "mprescripts")
		    sup = doc->getFormattingNode(node);
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
	      Ptr<MathMLElement> sub;
	      Ptr<MathMLElement> sup;

	      if (nodeLocalName(node) != "none")
		sub = doc->getFormattingNode(node);
	      i++;
	      
	      if (i < n)
		{
		  node = children.item(i);
		  if (nodeLocalName(node) != "none" && nodeLocalName(node) != "mprescripts")
		    sup = doc->getFormattingNode(node);
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
      
      if (n == 0 && !is_a<MathMLDummyElement>(base)) base = MathMLDummyElement::create();
      SetScriptsSize(nScripts);
      SetPreScriptsSize(nPreScripts);
#endif // HAVE_GMETADOM

      if (base) base->Normalize(doc);
      for_each_if(subScript.begin(), subScript.end(),
		  NotNullPredicate(), std::bind2nd(NormalizeAdaptor(), doc));
      for_each_if(superScript.begin(), superScript.end(),
		  NotNullPredicate(), std::bind2nd(NormalizeAdaptor(), doc));
      for_each_if(preSubScript.begin(), preSubScript.end(),
		  NotNullPredicate(), std::bind2nd(NormalizeAdaptor(), doc));
      for_each_if(preSuperScript.begin(), preSuperScript.end(),
		  NotNullPredicate(), std::bind2nd(NormalizeAdaptor(), doc));

      ResetDirtyStructure();
    }
}

void
MathMLMultiScriptsElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
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

      ResetDirtyAttribute();
    }
}

void
MathMLMultiScriptsElement::DoLayout(const class FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      assert(base);
      base->DoLayout(ctxt);

      BoundingBox subScriptBox;
      BoundingBox superScriptBox;
      scaled scriptsWidth = 0;
      scaled preScriptsWidth = 0;

      std::vector< Ptr<MathMLElement> >::const_iterator pSub;
      std::vector< Ptr<MathMLElement> >::const_iterator pSup;

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

	  subScriptBox.Append(subBox);
	  superScriptBox.Append(supBox);
	  scriptsWidth = scaledMax(scriptsWidth, scaledMax(subBox.width, supBox.width));
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

	  subScriptBox.Append(subBox);
	  superScriptBox.Append(supBox);
	  preScriptsWidth = scaledMax(preScriptsWidth, scaledMax(subBox.width, supBox.width));
	}

      DoScriptLayout(base->GetBoundingBox(), subScriptBox, superScriptBox,
		     subShiftX, subShiftY, superShiftX, superShiftY);
  
      box = base->GetBoundingBox();
      box.Append(preScriptsWidth + scriptsWidth);

      if (!subScriptBox.IsNull())
	{
	  box.ascent  = scaledMax(box.ascent, subScriptBox.ascent - subShiftY);
	  box.descent = scaledMax(box.descent, subScriptBox.descent + subShiftY);
	}

      if (!superScriptBox.IsNull())
	{
	  box.ascent  = scaledMax(box.ascent, superScriptBox.ascent + superShiftY);
	  box.descent = scaledMax(box.descent, superScriptBox.descent - superShiftY);
	}

      DoEmbellishmentLayout(this, box);

      ResetDirtyLayout(ctxt);
    }
}

void
MathMLMultiScriptsElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;

  SetEmbellishmentPosition(this, x, y);

  std::vector< Ptr<MathMLElement> >::reverse_iterator preSub;
  std::vector< Ptr<MathMLElement> >::reverse_iterator preSup;

  for (preSub = preSubScript.rbegin(), preSup = preSuperScript.rbegin();
       preSub != preSubScript.rend();
       preSub++, preSup++)
    {
      assert(preSup != preSuperScript.rend());
      scaled scriptW = 0;
      if (*preSub) 
	{
	  (*preSub)->SetPosition(x, y + subShiftY);
	  scriptW = (*preSub)->GetBoundingBox().GetWidth();
	}
      if (*preSup)
	{
	  (*preSup)->SetPosition(x, y - superShiftY);
	  scriptW = scaledMax(scriptW, (*preSup)->GetBoundingBox().GetWidth());
	}

      x += scriptW;
    }

  assert(base);
  base->SetPosition(x, y);

  std::vector< Ptr<MathMLElement> >::iterator pSub;
  std::vector< Ptr<MathMLElement> >::iterator pSup;

  for (pSub = subScript.begin(), pSup = superScript.begin();
       pSub != subScript.end();
       pSub++, pSup++)
    {
      assert(pSup != superScript.end());
      scaled scriptW = 0;
      if (*pSub) 
	{
	  (*pSub)->SetPosition(x + subShiftX, y + subShiftY);
	  scriptW = (*pSub)->GetBoundingBox().GetWidth();
	}
      if (*pSup)
	{
	  (*pSup)->SetPosition(x + superShiftX, y - superShiftY);
	  scriptW = scaledMax(scriptW, (*pSup)->GetBoundingBox().GetWidth());
	}

      x += scriptW;
    }
}

void
MathMLMultiScriptsElement::Render(const DrawingArea& area)
{
  if (Dirty())
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

Ptr<MathMLElement>
MathMLMultiScriptsElement::Inside(scaled x, scaled y)
{
  if (IsInside(x, y))
    {
      assert(base);
      if (Ptr<MathMLElement> inside = base->Inside(x, y)) return inside;

      for (std::vector< Ptr<MathMLElement> >::iterator elem = preSubScript.begin();
	   elem != preSubScript.end(); elem++)
	if (*elem)
	  {
	    Ptr<MathMLElement> inside = (*elem)->Inside(x, y);
	    if (inside) return inside;
	  }

      for (std::vector< Ptr<MathMLElement> >::iterator elem = preSuperScript.begin();
	   elem != preSuperScript.end(); elem++)
	if (*elem)
	  {
	    Ptr<MathMLElement> inside = (*elem)->Inside(x, y);
	    if (inside) return inside;
	  }

      for (std::vector< Ptr<MathMLElement> >::iterator elem = subScript.begin();
	   elem != subScript.end(); elem++)
	if (*elem)
	  {
	    Ptr<MathMLElement> inside = (*elem)->Inside(x, y);
	    if (inside) return inside;
	  }

      for (std::vector< Ptr<MathMLElement> >::iterator elem = superScript.begin();
	   elem != superScript.end(); elem++)
	if (*elem)
	  {
	    Ptr<MathMLElement> inside = (*elem)->Inside(x, y);
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
      Ptr<MathMLElement> sub = GetPreSubScript(preSubScript.size() - 1);
      Ptr<MathMLElement> sup = GetPreSuperScript(preSuperScript.size() - 1);
      if (sub && sup)
	return scaledMin(sub->GetLeftEdge(), sup->GetLeftEdge());
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
      Ptr<MathMLElement> sub = GetSubScript(subScript.size() - 1);
      Ptr<MathMLElement> sup = GetSuperScript(superScript.size() - 1);
      if (sub && sup)
	return scaledMin(sub->GetLeftEdge(), sup->GetLeftEdge());
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

Ptr<MathMLOperatorElement>
MathMLMultiScriptsElement::GetCoreOperator()
{
  if (base) return base->GetCoreOperator();
  else return 0;
}

#if 0
void
MathMLMultiScriptsElement::SetDirty(const Rectangle* rect)
{
  if (!IsDirty() && !HasDirtyChildren())
    {
      MathMLElement::SetDirty(rect);
      if (base) base->SetDirty(rect);
      for_each_if(subScript.begin(), subScript.end(), NotNullPredicate(), std::bind2nd(SetDirtyAdaptor(), rect));
      for_each_if(superScript.begin(), superScript.end(), NotNullPredicate(), std::bind2nd(SetDirtyAdaptor(), rect));
      for_each_if(preSubScript.begin(), preSubScript.end(), NotNullPredicate(), std::bind2nd(SetDirtyAdaptor(), rect));
      for_each_if(preSuperScript.begin(), preSuperScript.end(), NotNullPredicate(), std::bind2nd(SetDirtyAdaptor(), rect));
    }
}

void
MathMLMultiScriptsElement::SetDirtyLayout(bool children)
{
  MathMLElement::SetDirtyLayout(children);
  if (children)
    {
      if (base) base->SetDirtyLayout(children);
      for_each_if(subScript.begin(), subScript.end(), NotNullPredicate(), std::bind2nd(SetDirtyLayoutAdaptor(), true));
      for_each_if(superScript.begin(), superScript.end(), NotNullPredicate(), std::bind2nd(SetDirtyLayoutAdaptor(), true));
      for_each_if(preSubScript.begin(), preSubScript.end(), NotNullPredicate(), std::bind2nd(SetDirtyLayoutAdaptor(), true));
      for_each_if(preSuperScript.begin(), preSuperScript.end(), NotNullPredicate(), std::bind2nd(SetDirtyLayoutAdaptor(), true));
    }
}
#endif

void
MathMLMultiScriptsElement::SetFlagDown(Flags f)
{
  MathMLElement::SetFlagDown(f);
  if (base) base->SetFlagDown(f);
  for_each_if(subScript.begin(), subScript.end(), NotNullPredicate(), std::bind2nd(SetFlagDownAdaptor(), f));
  for_each_if(superScript.begin(), superScript.end(), NotNullPredicate(), std::bind2nd(SetFlagDownAdaptor(), f));
  for_each_if(preSubScript.begin(), preSubScript.end(), NotNullPredicate(), std::bind2nd(SetFlagDownAdaptor(), f));
  for_each_if(preSuperScript.begin(), preSuperScript.end(), NotNullPredicate(), std::bind2nd(SetFlagDownAdaptor(), f));
}

void
MathMLMultiScriptsElement::ResetFlagDown(Flags f)
{
  MathMLElement::ResetFlagDown(f);
  if (base) base->ResetFlagDown(f);
  for_each_if(subScript.begin(), subScript.end(), NotNullPredicate(), std::bind2nd(ResetFlagDownAdaptor(), f));
  for_each_if(superScript.begin(), superScript.end(), NotNullPredicate(), std::bind2nd(ResetFlagDownAdaptor(), f));
  for_each_if(preSubScript.begin(), preSubScript.end(), NotNullPredicate(), std::bind2nd(ResetFlagDownAdaptor(), f));
  for_each_if(preSuperScript.begin(), preSuperScript.end(), NotNullPredicate(), std::bind2nd(ResetFlagDownAdaptor(), f));
}

