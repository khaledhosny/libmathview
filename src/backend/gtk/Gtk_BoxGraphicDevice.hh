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

#ifndef __Gtk_BoxGraphicDevice_hh__
#define __Gtk_BoxGraphicDevice_hh__

#include <gtk/gtk.h>

#include "BoxGraphicDevice.hh"
#include "GObjectPtr.hh"

class Gtk_BoxGraphicDevice : public BoxGraphicDevice
{
protected:
  Gtk_BoxGraphicDevice(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&);
  virtual ~Gtk_BoxGraphicDevice();

public:
  static SmartPtr<Gtk_BoxGraphicDevice> create(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&);

  virtual void setFactory(const SmartPtr<class Gtk_AreaFactory>&);
  virtual AreaRef string(const class FormattingContext&, const String&, const scaled&) const;
  virtual AreaRef dummy(const class FormattingContext&) const;
  virtual AreaRef wrapper(const class FormattingContext&, const AreaRef&) const;

private:
  GObjectPtr<PangoContext> pango_context;
  SmartPtr<class Gtk_AreaFactory> gtk_factory;
};

#endif // __Gtk_MathGraphicDevice_hh__
