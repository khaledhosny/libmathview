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

#include <config.h>

#include <cassert>

#include "customXmlReader.hh"

customXmlReader::customXmlReader(c_customXmlReader* r, void* data)
  : reader(r), user_data(data)
{
  assert(reader);
}

customXmlReader::~customXmlReader()
{
  (*reader->free_data)(user_data);
  reader = 0;
  user_data = 0;
}

String
customXmlReader::fromReaderString(char* str) const
{
  if (str)
    {
      String res(str);
      (*reader->free_string)(str);
      return res;
    }
  else
    return String();
}

void
customXmlReader::getAttribute(int index, String& namespaceURI, String& name, String& value) const
{
  char* c_namespaceURI;
  char* c_name;
  char* c_value;
  (*reader->get_attribute_by_index)(user_data, index, &c_namespaceURI, &c_name, &c_value);
  namespaceURI = fromReaderString(c_namespaceURI);
  name = fromReaderString(c_name);
  value = fromReaderString(c_value);
}
