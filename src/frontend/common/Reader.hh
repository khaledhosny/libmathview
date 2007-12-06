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

#ifndef __Reader_hh__
#define __Reader_hh__

#include "String.hh"
#include "Object.hh"

class Reader : public Object
{
public:
  virtual unsigned nodeType(void) const = 0;
  virtual String name(void) const = 0;
  virtual String namespaceURI(void) const = 0;
  virtual String prefix(void) const = 0;
  virtual String value(void) const = 0;

  virtual bool hasAttribute(const String&) const = 0;
  virtual bool hasAttributeNS(const String&, const String&) const = 0;
  virtual unsigned attributeCount(void) const = 0;
  virtual String getAttribute(const String&) const = 0;
  virtual String getAttributeNS(const String&, const String&) const = 0;
  virtual String getAttributeNo(unsigned) const = 0;

  virtual bool more(void) const = 0;
  virtual void firstChild(void) = 0;
  virtual void parentNode(void) = 0;
  virtual void nextSibling(void) = 0;
};

#endif // __Reader_hh__
