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

#include "Adapters.hh"
#include "MathMLMultiScriptsElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLAttributeSignatures.hh"

MathMLMultiScriptsElement::MathMLMultiScriptsElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLContainerElement(context)
{ }

MathMLMultiScriptsElement::~MathMLMultiScriptsElement()
{ }

AreaRef
MathMLMultiScriptsElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      assert(false);
    }

  return getArea();
}

#if 0
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
	      subBox = (*pSub)->getBoundingBox();
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
#endif

SmartPtr<MathMLOperatorElement>
MathMLMultiScriptsElement::getCoreOperator()
{
  return getBase() ? getBase()->getCoreOperator() : 0;
}

void
MathMLMultiScriptsElement::setFlagDown(Flags f)
{
  MathMLContainerElement::setFlagDown(f);
  base.setFlagDown(f);
  subScript.setFlagDown(f);
  superScript.setFlagDown(f);
  preSubScript.setFlagDown(f);
  preSuperScript.setFlagDown(f);
}

void
MathMLMultiScriptsElement::resetFlagDown(Flags f)
{
  MathMLContainerElement::resetFlagDown(f);
  base.resetFlagDown(f);
  subScript.resetFlagDown(f);
  superScript.resetFlagDown(f);
  preSubScript.resetFlagDown(f);
  preSuperScript.resetFlagDown(f);
}

