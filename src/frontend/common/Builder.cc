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

#include "Builder.hh"
#include "MathMLNamespaceContext.hh"
#if ENABLE_BOXML
#include "BoxMLNamespaceContext.hh"
#endif // ENABLE_BOXML

Builder::Builder()
{ }

Builder::~Builder()
{ }

#if ENABLE_BOXML
void
Builder::setNamespaceContexts(const SmartPtr<MathMLNamespaceContext>& mmlContext,
			      const SmartPtr<BoxMLNamespaceContext>& bmlContext)
{
  mathmlContext = mmlContext;
  boxmlContext = bmlContext;
}
#else
void
Builder::setNamespaceContext(const SmartPtr<MathMLNamespaceContext>& mmlContext)
{
  mathmlContext = mmlContext;
}
#endif // ENABLE_BOXML

SmartPtr<MathMLNamespaceContext>
Builder::getMathMLNamespaceContext() const
{ return mathmlContext; }

#if ENABLE_BOXML
SmartPtr<BoxMLNamespaceContext>
Builder::getBoxMLNamespaceContext() const
{ return boxmlContext; }
#endif // ENABLE_BOXML
