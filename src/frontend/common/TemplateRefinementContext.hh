// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __TemplateRefinementContext_hh__
#define __TemplateRefinementContext_hh__

#include <list>

#include "Attribute.hh"
#include "AttributeSet.hh"

template <class Model>
class TemplateRefinementContext
{
public:
  TemplateRefinementContext(void) { }

  SmartPtr<Attribute>
  get(const class AttributeSignature& sig) const
  {
    for (typename std::list<Context>::const_iterator p = context.begin(); p != context.end(); p++)
      {
	const Context& c = *p;
	
	if (SmartPtr<Attribute> attr = c.attributes->get(ATTRIBUTE_ID_OF_SIGNATURE(sig)))
	  return attr;
	else if (Model::hasAttribute(c.elem, sig.name))
	  {
	    SmartPtr<Attribute> attr = Attribute::create(sig, Model::getAttribute(c.elem, sig.name));
	    c.attributes->set(attr);
	    return attr;
	  }
      }
    
    return 0;
  }
  
  void
  push(const typename Model::Element& elem)
  {
    assert(elem);
    context.push_front(Context(elem, AttributeSet::create()));
  }

  void pop(void)
  {
    assert(!context.empty());
    context.pop_front();
  }

private:
  struct Context
  {
    Context(const typename Model::Element& el, const SmartPtr<AttributeSet> al) : elem(el), attributes(al) { }

    typename Model::Element elem;
    SmartPtr<AttributeSet> attributes;
  };

  std::list<Context> context;
};

#endif // __TemplateRefinementContext_hh__
