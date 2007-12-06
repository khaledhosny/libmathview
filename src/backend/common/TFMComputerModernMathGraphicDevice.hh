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

#ifndef __TFMComputerModernMathGraphicDevice_hh__
#define __TFMComputerModernMathGraphicDevice_hh__

#include "MathGraphicDevice.hh"
#include "ComputerModernFamily.hh"

class GMV_MathView_EXPORT TFMComputerModernMathGraphicDevice : public MathGraphicDevice
{
protected:
  TFMComputerModernMathGraphicDevice(const SmartPtr<class AbstractLogger>&);
  virtual ~TFMComputerModernMathGraphicDevice();

public:
  void setFamily(const SmartPtr<class ComputerModernFamily>&);
  void setTFMManager(const SmartPtr<class TFMManager>&);

  static SmartPtr<TFMComputerModernMathGraphicDevice> create(const SmartPtr<class AbstractLogger>&);

  virtual scaled em(const class FormattingContext&) const;
  virtual scaled ex(const class FormattingContext&) const;
  virtual scaled defaultLineThickness(const class FormattingContext&) const;
  virtual scaled axis(const class FormattingContext&) const;
  virtual AreaRef glyph(const class FormattingContext&,
			const String&, const String&,
			unsigned long) const;

protected:
  SmartPtr<class TFM> getTFM(const class FormattingContext&, ComputerModernFamily::FontEncId) const;

private:
  SmartPtr<class ComputerModernFamily> family;
  SmartPtr<class TFMManager> tfmManager;
};

#endif // __TFMComputerModernMathGraphicDevice_hh__
