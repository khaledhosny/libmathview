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

#include "Adaptors.hh"
#include "ChildList.hh"
#include "Globals.hh"
#include "ValueConversion.hh"
#include "MathMLDocument.hh"
#include "MathMLDummyElement.hh"
#include "MathMLTableElement.hh"
#include "MathMLTableRowElement.hh"
#include "MathMLTableCellElement.hh"

MathMLTableRowElement::MathMLTableRowElement()
{
  rowIndex = 0;
}

#if defined(HAVE_GMETADOM)
MathMLTableRowElement::MathMLTableRowElement(const DOM::Element& node)
  : MathMLLinearContainerElement(node)
{
  rowIndex = 0;
}
#endif

MathMLTableRowElement::~MathMLTableRowElement()
{
}

void
MathMLTableRowElement::refine(AbstractRefinementContext& context)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, TableRow, rowalign);
      REFINE_ATTRIBUTE(context, TableRow, columnalign);
      REFINE_ATTRIBUTE(context, TableRow, groupalign);
      MathMLLinearContainerElement::refine(context);
    }
}

void
MathMLTableRowElement::Normalize(const SmartPtr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (GetDOMElement())
	{
	  ChildList children(GetDOMElement(), MATHML_NS_URI, "mtd");
	  unsigned n = children.get_length();

	  std::vector< SmartPtr<MathMLElement> > newContent;
	  newContent.reserve(n);
	  for (unsigned i = 0; i < n; i++)
	    {
	      DOM::Node node = children.item(i);
	      SmartPtr<MathMLElement> elem = doc->getFormattingNode(node);
	      assert(elem);
	      newContent.push_back(elem);
	    }
	  SwapChildren(newContent);
	}
#endif
      
      std::for_each(content.begin(), content.end(), std::bind2nd(NormalizeAdaptor(), doc));

      ResetDirtyStructure();
    }
}

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
  if (DirtyAttribute() || DirtyAttributeP())
    {
#endif
      assert(GetParent());
      SmartPtr<MathMLTableElement> mtable = smart_cast<MathMLTableElement>(GetParent());
      assert(mtable);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(TableRow, columnalign))
	mtable->SetupColumnAlignAux(value, rowIndex, 1, labeledRow);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(TableRow, rowalign))
	mtable->SetupRowAlignAux(value, rowIndex, labeledRow);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(TableRow, groupalign))
	mtable->SetupGroupAlignAux(value, rowIndex, 1);

      MathMLLinearContainerElement::Setup(env);
      ResetDirtyAttribute();
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

SmartPtr<MathMLElement>
MathMLTableRowElement::GetLabel(void) const
{
  return 0;
}

void
MathMLTableRowElement::SetDirtyStructure()
{
  assert(GetParent());
  assert(is_a<MathMLTableElement>(GetParent()));
  SmartPtr<MathMLTableElement> table = smart_cast<MathMLTableElement>(GetParent());
  assert(table);
  table->SetDirtyStructure();
  MathMLLinearContainerElement::SetDirtyStructure();
}

void
MathMLTableRowElement::SetDirtyAttribute()
{
  assert(GetParent());
  assert(is_a<MathMLTableElement>(GetParent()));
  SmartPtr<MathMLTableElement> table = smart_cast<MathMLTableElement>(GetParent());
  assert(table);
  table->SetDirtyAttribute();
  MathMLLinearContainerElement::SetDirtyAttribute();
}

void
MathMLTableRowElement::SetDirtyLayout()
{
  assert(GetParent());
  assert(is_a<MathMLTableElement>(GetParent()));
  SmartPtr<MathMLTableElement> table = smart_cast<MathMLTableElement>(GetParent());
  assert(table);
  table->SetDirtyLayout();
  MathMLLinearContainerElement::SetDirtyLayout();
}
