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

#ifndef __AbstractReader_hh__
#define __AbstractReader_hh__

#include "String.hh"

class AbstractReader
{
public:
  virtual String name(void) const = 0;
  virtual String localName(void) const = 0;
  virtual String namespaceURI(void) const = 0;
  virtual String prefix(void) const = 0;
  virtual String value(void) const = 0;

  virtual unsigned attributeCount(void) const = 0;
  virtual String getAttribute(const String&) const = 0;
  virtual String getAttributeNS(const String&, const String&) const = 0;
  virtual String getAttributeNo(unsigned) const = 0;

  virtual unsigned nodeType(void) const = 0;

  virtual bool more(void) const = 0;
  virtual void firstChild(void) = 0;
  virtual void parentNode(void) = 0;
  virtual void nextSibling(void) = 0;
};

#endif // __AbstractReader_hh__
