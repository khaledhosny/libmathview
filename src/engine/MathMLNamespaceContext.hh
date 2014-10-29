// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#ifndef __MathMLNamespaceContext_hh__
#define __MathMLNamespaceContext_hh__

#include "NamespaceContext.hh"

class MathMLNamespaceContext : public NamespaceContext
{
protected:
  MathMLNamespaceContext(const SmartPtr<class View>&,
			 const SmartPtr<class MathGraphicDevice>&);
  virtual ~MathMLNamespaceContext();

public:
  static SmartPtr<MathMLNamespaceContext> create(const SmartPtr<class View>& view,
						 const SmartPtr<class MathGraphicDevice>& device)
  { return new MathMLNamespaceContext(view, device); }

  SmartPtr<class MathGraphicDevice> getGraphicDevice(void) const;

private:
  SmartPtr<class MathGraphicDevice> device;
};

#endif // __MathMLNamespaceContext_hh__
