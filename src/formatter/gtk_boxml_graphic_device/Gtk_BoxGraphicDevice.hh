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

#ifndef __Gtk_BoxGraphicDevice_hh__
#define __Gtk_BoxGraphicDevice_hh__

#include <gtk/gtk.h>

#include "BoxGraphicDevice.hh"
#include "GObjectPtr.hh"

class Gtk_BoxGraphicDevice : public BoxGraphicDevice
{
protected:
  Gtk_BoxGraphicDevice(GtkWidget*);
  virtual ~Gtk_BoxGraphicDevice();

public:
  static SmartPtr<Gtk_BoxGraphicDevice> create(GtkWidget* widget)
  { return new Gtk_BoxGraphicDevice(widget); }

  virtual SmartPtr<class AreaFactory> getFactory(void) const;
  virtual AreaRef string(const class BoxFormattingContext&, const String&, const scaled&) const;
  virtual AreaRef wrapper(const class BoxFormattingContext&, const AreaRef&) const;

private:
  GObjectPtr<PangoContext> pango_context;
  SmartPtr<class Gtk_AreaFactory> factory;
};

#endif // __Gtk_MathGraphicDevice_hh__
