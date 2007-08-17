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

#ifndef __MathMLNamespaceContext_hh__
#define __MathMLNamespaceContext_hh__

#include "NamespaceContext.hh"

class GMV_MathView_EXPORT MathMLNamespaceContext : public NamespaceContext
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
