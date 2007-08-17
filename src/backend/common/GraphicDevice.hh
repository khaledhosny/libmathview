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

#ifndef __GraphicDevice_hh__
#define __GraphicDevice_hh__

#include "Object.hh"
#include "Length.hh"
#include "scaled.hh"
#include "AreaFactory.hh"

class GMV_MathView_EXPORT GraphicDevice : public Object
{
protected:
  GraphicDevice(const SmartPtr<class AbstractLogger>&);
  virtual ~GraphicDevice();

public:
  SmartPtr<class AbstractLogger> getLogger(void) const;

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
  SmartPtr<class AbstractLogger> logger;
  SmartPtr<AreaFactory> factory;
};

#endif // __GraphicDevice_hh__
