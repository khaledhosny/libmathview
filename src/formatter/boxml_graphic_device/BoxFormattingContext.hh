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

#ifndef __BoxFormattingContext_hh__
#define __BoxFormattingContext_hh__

#include <string>

#include "scaled.hh"
#include "Variant.hh"
#include "RGBColor.hh"
#include "FormattingContext.hh"

class BoxFormattingContext : public FormattingContext
{
public:
  BoxFormattingContext(const SmartPtr<class BoxGraphicDevice>&);

  enum PropertyId {
    SIZE,
    COLOR,
    BACKGROUND_COLOR,
    ELEMENT,
    
    LAST_NAMED_PROPERTY_ENTRY
  };
  
  SmartPtr<class BoxGraphicDevice> getDevice(void) const;

  scaled getSize(void) const { return get<scaled>(SIZE); }
  void setSize(const scaled& s) { set<scaled>(SIZE, s); }
  RGBColor getColor(void) const { return get<RGBColor>(COLOR); }
  void setColor(const RGBColor& c) { set<RGBColor>(COLOR, c); }
  RGBColor getBackground(void) const { return get<RGBColor>(BACKGROUND_COLOR); }
  void setBackground(const RGBColor& c) { set<RGBColor>(BACKGROUND_COLOR, c); }
  SmartPtr<class BoxMLElement> getElement(void) const { return get< SmartPtr<class BoxMLElement> >(ELEMENT); }

  void push(const SmartPtr<class BoxMLElement>& el)
  {
    FormattingContext::push();
    set(ELEMENT, el);
  }
  
private:
  SmartPtr<class BoxGraphicDevice> device;
};

#endif // __BoxFormattingContext_hh__
