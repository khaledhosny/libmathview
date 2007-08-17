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

#ifndef __libxml2_Builder_hh__
#define __libxml2_Builder_hh__

#include "libxml2_Model.hh"
#include "TemplateLinker.hh"
#include "Builder.hh"
#include "String.hh"
#include "Element.hh"

class libxml2_Builder : public Builder
{
protected:
  libxml2_Builder(void) : root(0) { }
  virtual ~libxml2_Builder();

public:
  static SmartPtr<libxml2_Builder> create(void);

  void setRootModelElement(xmlElement*);
  xmlElement* getRootModelElement(void) const { return root; }

  SmartPtr<Element> findElement(xmlElement* p) const { return linker.assoc(p); }
  xmlElement* findSelfOrAncestorModelElement(const SmartPtr<Element>&) const;
  SmartPtr<Element> findSelfOrAncestorElement(xmlElement*) const;

  bool notifyStructureChanged(xmlElement*);
  bool notifyAttributeChanged(xmlElement*, const xmlChar*);

protected:
  // methods for accessing the linker
  SmartPtr<Element> linkerAssoc(xmlElement* el) const { return linker.assoc(el); }
  void linkerAdd(xmlElement* el, Element* elem) const { linker.add(el, elem); }
  void linkerRemove(Element* elem) const { linker.remove(elem); }

private:
  mutable TemplateLinker<libxml2_Model> linker;
  xmlElement* root;
};

#endif // __libxml2_Builder_hh__
