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

#ifndef __libxml2_reader_Builder_hh__
#define __libxml2_reader_Builder_hh__

#include "libxmlXmlReader.hh"
#include "TemplateReaderBuilder.hh"
#include "String.hh"
#include "Element.hh"

class libxml2_reader_Builder : public TemplateReaderBuilder<libxmlXmlReader>
{
protected:
  libxml2_reader_Builder(void) { }
  virtual ~libxml2_reader_Builder() { }

public:
  static SmartPtr<libxml2_reader_Builder> create(void);

protected:
  SmartPtr<Element> linkerAssoc(const SmartPtr<libxmlXmlReader>&) const { return 0; }
  void linkerAdd(const SmartPtr<libxmlXmlReader>&, Element*) const { }
  void linkerRemove(Element*) const { }
};

#endif // __libxml2_reader_Builder_hh__
