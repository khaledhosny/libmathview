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

#ifndef __GraphicDevice_hh__
#define __GraphicDevice_hh__

#include "Object.hh"
#include "Length.hh"
#include "scaled.hh"
#include "AreaFactory.hh"

class GraphicDevice : public Object
{
protected:
  GraphicDevice(void);
  virtual ~GraphicDevice();

public:
  virtual void setFactory(const SmartPtr<AreaFactory>&);
  SmartPtr<AreaFactory> getFactory(void) const { return factory; }
  virtual void setShaperManager(const SmartPtr<class ShaperManager>&);
  SmartPtr<ShaperManager> getShaperManager(void) const;

  // Length evaluation, fundamental properties
  virtual double dpi(const class FormattingContext&) const;
  virtual scaled evaluate(const class FormattingContext&, const Length&, const scaled&) const;
  virtual scaled em(const class FormattingContext&) const;
  virtual scaled ex(const class FormattingContext&) const = 0;
  virtual scaled defaultLineThickness(const class FormattingContext&) const;

protected:
 SmartPtr<class ShaperManager> shaperManager;

private:
  SmartPtr<AreaFactory> factory;
};

#endif // __GraphicDevice_hh__
