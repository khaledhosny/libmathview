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

#include "BoxMLMathMLAdapter.hh"
#include "BoxMLAttributeSignatures.hh"
#include "BoxFormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "BoxMLNamespaceContext.hh"
#include "View.hh"
#include "NamespaceRegistry.hh"

#include "MathMLElement.hh"
#include "MathFormattingContext.hh"
#include "MathMLNamespaceContext.hh"
#include "MathGraphicDevice.hh"

BoxMLMathMLAdapter::BoxMLMathMLAdapter(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLElement(context)
{ }

BoxMLMathMLAdapter::~BoxMLMathMLAdapter()
{ }

SmartPtr<BoxMLMathMLAdapter>
BoxMLMathMLAdapter::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLMathMLAdapter(context); }

AreaRef
BoxMLMathMLAdapter::format(BoxFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      if (SmartPtr<MathMLElement> child = getChild())
	{
	  SmartPtr<MathMLNamespaceContext> contentContext = child->getMathMLNamespaceContext();
	  assert(contentContext);

	  MathFormattingContext mCtxt(contentContext->getGraphicDevice());
	  mCtxt.setSize(ctxt.getSize());
	  mCtxt.setActualSize(ctxt.getSize());
	  mCtxt.setColor(ctxt.getColor());
	  mCtxt.setBackground(ctxt.getBackground());

	  AreaRef res = child->format(mCtxt);
	  res = ctxt.getDevice()->wrapper(ctxt, res);
	  setArea(res);
	}
      else
	setArea(ctxt.getDevice()->dummy(ctxt));
      
      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
