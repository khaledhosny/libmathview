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

#include "AbstractRefinementContext.hh"
#include "Globals.hh"
#include "AttributeList.hh"
#include "MathMLElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLStyleElement.hh"
#include "MathMLView.hh"
#include "ValueConversion.hh"
#include "Variant.hh"
#include "traverseAux.hh"

MathMLElement::MathMLElement(const SmartPtr<MathMLView>& v) : view(v)
{
  assert(view);
}

MathMLElement::~MathMLElement()
{ }

#if 0
void
MathMLElement::construct(AbstractConstructionContext&)
{
  // nothing to construct
  resetDirtyStructure();
}
#endif

void
MathMLElement::construct()
{
  // nothing to construct
  resetDirtyStructure();
}

void
MathMLElement::refine(class AbstractRefinementContext&)
{
  // nothing to refine
  resetDirtyAttribute();
}

AreaRef
MathMLElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      setArea(0);
      resetDirtyLayout();
    }

  return getArea();
}

bool
MathMLElement::IsSpaceLike() const
{
  return false;
}

SmartPtr<MathMLOperatorElement>
MathMLElement::getCoreOperator()
{
  return 0;
}

SmartPtr<MathMLOperatorElement>
MathMLElement::getCoreOperatorTop()
{
  if (SmartPtr<MathMLOperatorElement> coreOp = getCoreOperator())
    {
      SmartPtr<MathMLElement> parent = smart_cast<MathMLElement>(getParent());
      if (!parent || parent->getCoreOperator() != coreOp)
	return coreOp;
    }
  return 0;
}

#if defined(HAVE_GMETADOM)
void
MathMLElement::refineAttribute(const AbstractRefinementContext& context, const AttributeSignature& signature)
{
  SmartPtr<Attribute> attr;
  
  if (signature.fromElement)
    if (DOM::Element el = getDOMElement())
      if (el.hasAttribute(signature.name))
	attr = Attribute::create(signature, el.getAttribute(signature.name));

  if (!attr && signature.fromContext)
    attr = context.get(signature);

  if (attr) setAttribute(attr);
  else removeAttribute(signature);
}

TokenId
MathMLElement::IsA() const
{
  if (DOM::Element el = getDOMElement())
    return tokenIdOfString(std::string(nodeLocalName(el)).c_str());
  else
    return T__NOTVALID;
}

bool
MathMLElement::IsSet(TokenId id) const
{
  return getDOMElement() && getDOMElement().hasAttribute(stringOfTokenId(id));
}
#endif // HAVE_GMETADOM

SmartPtr<MathMLView>
MathMLElement::getView() const
{
  return static_cast<MathMLView*>(view);
}

