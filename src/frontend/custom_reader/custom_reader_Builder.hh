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

#ifndef __custom_reader_Builder_hh__
#define __custom_reader_Builder_hh__

#include "customXmlReader.hh"
#include "TemplateReaderBuilder.hh"
#include "custom_reader_Model.hh"
#include "String.hh"
#include "Element.hh"

class custom_reader_Builder : public TemplateReaderBuilder<customXmlReader>
{
protected:
  custom_reader_Builder(void) { }
  virtual ~custom_reader_Builder() { }

public:
  static SmartPtr<custom_reader_Builder> create(void);

  SmartPtr<Element> findElement(void* p) const { return linker.assoc(p); }
  void* findSelfOrAncestorModelElement(const SmartPtr<Element>&) const;
  SmartPtr<Element> findSelfOrAncestorElement(void* p) const { return findElement(p); }

  bool notifyStructureChanged(void*);
  bool notifyAttributeChanged(void*, const char*);

protected:
  SmartPtr<Element>
  linkerAssoc(const SmartPtr<customXmlReader>& reader) const
  {
    if (void* id = reader->getNodeId())
      return linker.assoc(id);
    else
      return 0;
  }

  void
  linkerAdd(const SmartPtr<customXmlReader>& reader, Element* elem) const
  { if (void* id = reader->getNodeId()) linker.add(id, elem); }

  void linkerRemove(Element* elem) const { linker.remove(elem); }

private:
  mutable custom_reader_Model::Linker linker;
};

#endif // __custom_reader_Builder_hh__
