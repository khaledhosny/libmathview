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

#include <config.h>

#include <cassert>

#include "customXmlReader.hh"

customXmlReader::customXmlReader(const c_customXmlReader* r, c_customModelUserData data)
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
