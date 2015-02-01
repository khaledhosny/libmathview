// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#include <config.h>

#include <cassert>

#include <algorithm>

#include "Adapters.hh"
#include "ValueConversion.hh"
#include "MathMLTableElement.hh"
#include "MathMLTableRowElement.hh"
#include "MathMLTableCellElement.hh"
#include "MathMLAttributeSignatures.hh"

MathMLTableRowElement::MathMLTableRowElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLLinearContainerElement(context)
{
  rowIndex = 0;
}

MathMLTableRowElement::~MathMLTableRowElement()
{ }

#if 0
void
MathMLTableRowElement::construct()
{
  if (dirtyStructure())
    {
      
      std::for_each(content.begin(), content.end(), ConstructAdapter<MathMLElement>());

      resetDirtyStructure();
    }
}

void
MathMLTableRowElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, MathML, TableRow, rowalign);
      REFINE_ATTRIBUTE(context, MathML, TableRow, columnalign);
      REFINE_ATTRIBUTE(context, MathML, TableRow, groupalign);
      MathMLLinearContainerElement::refine(context);
    }
}
#endif

#if 0
void
MathMLTableRowElement::SetupRowIndex(unsigned i)
{
  rowIndex = i;
}

void
MathMLTableRowElement::SetupCellSpanning(RenderingEnvironment& env)
{
  for (std::vector< SmartPtr<MathMLElement> >::iterator p = content.begin();
       p != content.end();
       p++)
    {
      assert(is_a<MathMLTableCellElement>(*p));
      SmartPtr<MathMLTableCellElement> mtd = smart_cast<MathMLTableCellElement>(*p);
      assert(mtd);
      mtd->SetupCellSpanning(env);
    }
}

void
MathMLTableRowElement::Setup(RenderingEnvironment& env)
{
  SetupAux(env, false);
}

void
MathMLTableRowElement::SetupAux(RenderingEnvironment& env, bool labeledRow)
{
#if 0
  if (dirtyAttribute() || dirtyAttributeP())
    {
#endif
      SmartPtr<MathMLTableElement> mtable = getParent<MathMLTableElement>();
      assert(mtable);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(TableRow, columnalign))
	mtable->SetupColumnAlignAux(value, rowIndex, 1, labeledRow);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(TableRow, rowalign))
	mtable->SetupRowAlignAux(value, rowIndex, labeledRow);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(TableRow, groupalign))
	mtable->SetupGroupAlignAux(value, rowIndex, 1);

      MathMLLinearContainerElement::Setup(env);
      resetDirtyAttribute();
#if 0
    }
#endif
}

bool
MathMLTableRowElement::IsInside(const scaled& x, const scaled& y) const
{
  // same arguments as for the SetDirty method above
  for (std::vector< SmartPtr<MathMLElement> >::const_iterator elem = content.begin();
       elem != content.end();
       elem++)
    {
      assert(*elem);
      if ((*elem)->IsInside(x, y)) return true;
    }

  return false;
}
#endif

SmartPtr<MathMLElement>
MathMLTableRowElement::GetLabel(void) const
{
  return nullptr;
}

void
MathMLTableRowElement::setDirtyStructure()
{
  SmartPtr<MathMLTableElement> table = getParent<MathMLTableElement>();
  assert(table);
  table->setDirtyStructure();
  MathMLLinearContainerElement::setDirtyStructure();
}

void
MathMLTableRowElement::setDirtyAttribute()
{
  SmartPtr<MathMLTableElement> table = getParent<MathMLTableElement>();
  assert(table);
  table->setDirtyAttribute();
  MathMLLinearContainerElement::setDirtyAttribute();
}

void
MathMLTableRowElement::setDirtyLayout()
{
  SmartPtr<MathMLTableElement> table = getParent<MathMLTableElement>();
  assert(table);
  table->setDirtyLayout();
  MathMLLinearContainerElement::setDirtyLayout();
}
