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

#include "traverseAux.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLScriptCommonElement.hh"

MathMLScriptCommonElement::MathMLScriptCommonElement()
{
}

void
MathMLScriptCommonElement::SetBase(const Ptr<MathMLElement>& elem)
{
  if (elem != base)
    {
      
    }
}

void
MathMLScriptCommonElement::ScriptSetup(RenderingEnvironment& env)
{
  ruleThickness = env.GetRuleThickness();
#ifdef TEXISH_MATHML
  sppex = env.GetScaledPointsPerEx();
  subMinShift = float2sp(sp2float(env.GetFontAttributes().size.ToScaledPoints()) * 0.247217);
  superMinShift = float2sp(sp2float(env.GetFontAttributes().size.ToScaledPoints()) * 0.362892);
#else
  sppex = subMinShift = superMinShift = env.GetAxis();
#endif // TEXISH_MATHML
  scriptAxis    = env.GetAxis();
}

void
MathMLScriptCommonElement::DoScriptLayout(const BoundingBox& baseBox,
					  const BoundingBox& subScriptBox,
					  const BoundingBox& superScriptBox,
					  scaled& subShiftX, scaled& subShiftY,
					  scaled& superShiftX, scaled& superShiftY)
{
  assert(base);

  scaled u;
  scaled v;

  Ptr<MathMLElement> rel = findRightmostChild(base);
  assert(rel);

  Ptr<MathMLOperatorElement> coreOp = rel->GetCoreOperator();

  if ((is_a<MathMLTokenElement>(rel) && !coreOp) ||
      (coreOp && !coreOp->IsStretchy() && coreOp->IsFence()))
    {
      u = v = 0;
    } 
  else
    {
      u = baseBox.ascent - scriptAxis;
      v = baseBox.descent + scriptAxis / 2;
    }

  if (superScriptBox.IsNull())
    {
      u = 0;
      v = scaledMax(v, scaledMax(subMinShift, subScriptBox.ascent - (4 * sppex) / 5));
    } 
  else
    {
      u = scaledMax(u, scaledMax(superMinShift, superScriptBox.descent + sppex / 4));

      if (subScriptBox.IsNull())
	{
	  v = 0;
	} 
      else
	{
	  v = scaledMax(v, subMinShift);

	  if ((u - superScriptBox.descent) - (subScriptBox.ascent - v) < 4 * ruleThickness)
	    {
	      v = 4 * ruleThickness - u + superScriptBox.descent + subScriptBox.ascent;

	      scaled psi = (4 * sppex) / 5 - (u - superScriptBox.descent);
	      if (psi > 0)
		{
		  u += psi;
		  v -= psi;
		}
	    }
	}
    }

  superShiftY = u;
  superShiftX = scaledMax(baseBox.width, baseBox.rBearing + sppex / 5);

  subShiftY = v;
  subShiftX = baseBox.width;
}
