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

#ifndef __gmetadom_Reader_hh__
#define __gmetadom_Reader_hh__

#include "DOM.hh"
#include "Reader.hh"

class gmetadom_Reader : public Reader
{
public:
  gmetadom_Reader(void);
  virtual ~gmetadom_Reader();

  void reset(const DOM::Node& n) { setStatus(DOM::Node(), n, true); }
  bool valid(void) const;
  DOM::Node getNode(void) const { return node; }

  virtual unsigned nodeType(void) const;
  virtual String name(void) const;
  virtual String namespaceURI(void) const;
  virtual String prefix(void) const;
  virtual String value(void) const;

  virtual bool hasAttribute(const String&) const;
  virtual bool hasAttributeNS(const String&, const String&) const;
  virtual unsigned attributeCount(void) const;
  virtual String getAttribute(const String&) const;
  virtual String getAttributeNS(const String&, const String&) const;
  virtual String getAttributeNo(unsigned) const;

  virtual bool more(void) const;
  virtual void firstChild(void);
  virtual void parentNode(void);
  virtual void nextSibling(void);

protected:
  DOM::NamedNodeMap getAttributes(void) const;
  void setStatus(const DOM::Node&, const DOM::Node&, bool);

private:
  DOM::Node parent;
  DOM::Node node;
  mutable DOM::NamedNodeMap aMap;
  bool fresh;
};

#endif // __gmetadom_Reader_hh__
