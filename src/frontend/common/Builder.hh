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

#ifndef __Builder_hh__
#define __Builder_hh__

#include "Object.hh"
#include "SmartPtr.hh"

class GMV_MathView_EXPORT Builder : public Object
{
protected:
  Builder(void);
  virtual ~Builder();

public:
  virtual SmartPtr<class Element> getRootElement(void) const = 0;
  virtual void forgetElement(Element*) const = 0;

  void setLogger(const SmartPtr<class AbstractLogger>&);
  SmartPtr<class AbstractLogger> getLogger(void) const;

  void setMathMLNamespaceContext(const SmartPtr<class MathMLNamespaceContext>&);
  SmartPtr<class MathMLNamespaceContext> getMathMLNamespaceContext(void) const;
#if GMV_ENABLE_BOXML
  void setBoxMLNamespaceContext(const SmartPtr<class BoxMLNamespaceContext>&);
  SmartPtr<class BoxMLNamespaceContext> getBoxMLNamespaceContext(void) const;
#endif // GMV_ENABLE_BOXML

protected:
  SmartPtr<class AbstractLogger> logger;
  SmartPtr<class MathMLNamespaceContext> mathmlContext;
#if GMV_ENABLE_BOXML
  SmartPtr<class BoxMLNamespaceContext> boxmlContext;
#endif // GMV_ENABLE_BOXML
};

#endif // __Builder_hh__
