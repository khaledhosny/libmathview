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
