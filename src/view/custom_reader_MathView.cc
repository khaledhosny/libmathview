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

#include "custom_reader_MathView.hh"
#include "custom_reader_Builder.hh"

custom_reader_MathView::custom_reader_MathView()
  : data(0)
{
  setBuilder(custom_reader_Builder::create());
}

custom_reader_MathView::~custom_reader_MathView()
{ }

void
custom_reader_MathView::unload()
{ }

bool
custom_reader_MathView::loadReader(const c_customXmlReader* reader,
				   c_customModelUserData data)
{
  if (SmartPtr<custom_reader_Builder> builder = smart_cast<custom_reader_Builder>(getBuilder()))
    {
      builder->setReader(customXmlReader::create(reader, data));
      return true;
    }

  unload();
  return false;
}

SmartPtr<Element>
custom_reader_MathView::elementOfModelElement(c_customModelElementId el) const
{
  if (SmartPtr<custom_reader_Builder> builder = smart_cast<custom_reader_Builder>(getBuilder()))
    return builder->findElement(el);
  else
    return 0;
}

c_customModelElementId
custom_reader_MathView::modelElementOfElement(const SmartPtr<Element>& elem) const
{
  if (SmartPtr<custom_reader_Builder> builder = smart_cast<custom_reader_Builder>(getBuilder()))
    return builder->findSelfOrAncestorModelElement(elem);
  else
    return 0;
}

bool
custom_reader_MathView::notifyStructureChanged(c_customModelElementId) const
{ return false; }

bool
custom_reader_MathView::notifyAttributeChanged(c_customModelElementId, const char*) const
{ return false; }

