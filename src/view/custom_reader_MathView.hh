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

#ifndef __custom_reader_MathView_hh__
#define __custom_reader_MathView_hh__

#include "View.hh"
#include "libxml/tree.h"

#include "c_customXmlReader.h"

class custom_reader_MathView : public View
{
protected:
  custom_reader_MathView(void);
  virtual ~custom_reader_MathView();

public:
  static SmartPtr<custom_reader_MathView> create(void) { return new custom_reader_MathView(); }

  void unload(void);
  bool loadReader(const c_customXmlReader*, c_customModelUserData);

  bool notifyStructureChanged(c_customModelElementId) const;
  bool notifyAttributeChanged(c_customModelElementId, const char*) const;
  c_customModelElementId modelElementOfElement(const SmartPtr<class Element>&) const;
  SmartPtr<class Element> elementOfModelElement(c_customModelElementId) const;

  c_customModelUserData getDocument(void) const { return data; }

  virtual bool loadConfiguration(const SmartPtr<class Configuration>&, const String&) const;
  virtual bool loadOperatorDictionary(const SmartPtr<class MathMLOperatorDictionary>&, const String&) const;

protected:
  c_customModelUserData data;
};

#endif // __custom_reader_MathView_hh__
