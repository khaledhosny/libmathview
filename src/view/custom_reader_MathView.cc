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

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "custom_reader_MathView.hh"
#include "custom_reader_Builder.hh"

custom_reader_MathView::custom_reader_MathView(const SmartPtr<AbstractLogger>& logger)
  : View(logger), data(0)
{
  setBuilder(custom_reader_Builder::create());
}

custom_reader_MathView::~custom_reader_MathView()
{ }

SmartPtr<custom_reader_MathView>
custom_reader_MathView::create(const SmartPtr<AbstractLogger>& logger)
{ return new custom_reader_MathView(logger); }

void
custom_reader_MathView::unload()
{ }

bool
custom_reader_MathView::loadReader(const c_customXmlReader* reader,
				   c_customModelUserData data)
{
  if (SmartPtr<custom_reader_Builder> builder = smart_cast<custom_reader_Builder>(getBuilder()))
    {
      resetRootElement();
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

