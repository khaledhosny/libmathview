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

#ifndef __gmetadom_Builder_hh__
#define __gmetadom_Builder_hh__

#include "Builder.hh"
#include "RefinementContext.hh"

class gmetadom_Builder : public Builder
{
public:
  virtual bool more(void) const;
  virtual void beginContent(void);
  virtual void endContent(void);
  virtual void pushRefinementContext(void);
  virtual void dropRefinementContext(void);
  virtual NamespaceID getElementNamespaceID(void) const;
  virtual String getElementNamespace(void) const;
  virtual String getElementTag(void) const;
  virtual SmartPtr<Attribute> getAttribute(const AttributeSignature&) const;

protected:
  virtual SmartPtr<Element> getCachedElement(void) const;
  virtual SmartPtr<Element> getDummyElement(void) const;
  virtual SmartPtr<Attribute> getAttributeFromContext(const AttributeSignature&) const;
  virtual SmartPtr<ElementBuilder> getElementBuilder(void) const;

private:
  SmartPtr<class Linker> linker;
  SmartPtr<class gmetadom_Reader> reader;
  RefinementContext refinementContext;
};

#endif // __gmetadom_Builder_hh__
