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

#ifndef __gmetadomIterators_hh__
#define __gmetadomIterators_hh__

#include "gmetadom.hh"
#include "String.hh"

namespace GdomeSmartDOMExt
{

  class ChildNodesIterator
  {
  public:
    ChildNodesIterator(const DOM::Node& n) : p(n.get_firstChild()) { }
  
    DOM::Node node(void);

  private:
    DOM::Node p;
  };

  class ChildElementsIterator
  {
  public:
    ChildElementsIterator(const DOM::Node& n, const DOM::GdomeString& = "*", const DOM::GdomeString& = "*");

    DOM::Element element(void);

  protected:
    void next(void);

  private:
    DOM::Node p;
    DOM::GdomeString ns;
    DOM::GdomeString tag;
  };

}

#endif // __gmetadomIterators_hh__
