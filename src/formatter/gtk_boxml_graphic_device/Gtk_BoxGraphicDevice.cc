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

#include "Gtk_AreaFactory.hh"
#include "Gtk_BoxGraphicDevice.hh"
#include "BoxFormattingContext.hh"
#include "BoxMLElement.hh"

Gtk_BoxGraphicDevice::Gtk_BoxGraphicDevice(GtkWidget* widget)
{
  factory = Gtk_AreaFactory::create();
  pango_context = gtk_widget_create_pango_context(widget);
}

Gtk_BoxGraphicDevice::~Gtk_BoxGraphicDevice()
{ }

SmartPtr<AreaFactory>
Gtk_BoxGraphicDevice::getFactory() const
{
  return factory;
}

AreaRef
Gtk_BoxGraphicDevice::string(const BoxFormattingContext& context,
			     const String& str) const
{
  const DOM::UTF8String utf8_string = str;

  PangoLayout* layout = pango_layout_new(pango_context);
  pango_layout_set_text(layout, utf8_string.data(), utf8_string.length());
  // set attributes...
  return factory->pangoLayout(layout);
}

AreaRef
Gtk_BoxGraphicDevice::wrapper(const BoxFormattingContext& context,
			      const AreaRef& base) const
{
  BoundingBox box = base->box();
  return factory->wrapper(base->fit(box.width, box.height, box.depth), box, context.getElement());
}
