// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#ifndef __Builder_hh__
#define __Builder_hh__

#include "Object.hh"
#include "HashMap.hh"
#include "SmartPtr.hh"
#include "String.hh"

class Builder
{
public:
  Builder(void);
  virtual ~Builder();

  enum NamespaceID {
    NOTVALID_NS_ID = -1,
    MATHML_NS_ID,
    BOXML_NS_ID,

    LAST_NS_ID
  };

  struct ElementBuilder : public Object
  {
    virtual SmartPtr<class Element> create(void) const = 0;
    virtual void refine(const Builder&, const SmartPtr<class Element>&) const = 0;
    virtual void update(const Builder&, const SmartPtr<class Element>&) const = 0;
  };

  void registerElementBuilder(const String&, const String&, const SmartPtr<ElementBuilder>&);
  void registerDummyElementBuilder(const String&, const SmartPtr<ElementBuilder>&);
  SmartPtr<class Element> getElement(void);

  virtual bool more(void) const = 0;
  virtual void beginContent(void) = 0;
  virtual void endContent(void) = 0;
  virtual void pushRefinementContext(void) = 0;
  virtual void dropRefinementContext(void) = 0;
  virtual NamespaceID getElementNamespaceID(void) const = 0;
  virtual String getElementNamespace(void) const = 0;
  virtual String getElementTag(void) const = 0;
  virtual SmartPtr<class Attribute> getAttribute(const class AttributeSignature&) const = 0;

protected:
  virtual SmartPtr<class Element> getCachedElement(void) const = 0;
  virtual SmartPtr<class Element> getDummyElement(void) const = 0;
  virtual SmartPtr<class Attribute> getAttributeFromContext(const class AttributeSignature&) const = 0;
  virtual SmartPtr<ElementBuilder> getElementBuilder(void) const = 0;

  void refineAttribute(const SmartPtr<class Element>&, const class AttributeSignature&) const;
  SmartPtr<class Element> getElementNoCreate(void);
  SmartPtr<ElementBuilder> getDummyElementBuilder(const String&) const;
  SmartPtr<ElementBuilder> getDummyElementBuilder(NamespaceID) const;

private:
  typedef HASH_MAP_NS::hash_map<String,NamespaceID,StringHash,StringEq> NamespaceMap;
  NamespaceMap ns_map;
  HASH_MAP_NS::hash_map<String,SmartPtr<ElementBuilder>,StringHash,StringEq> builder[LAST_NS_ID];
  SmartPtr<ElementBuilder> dummyBuilder[LAST_NS_ID];
};

#endif // __Builder_hh__
