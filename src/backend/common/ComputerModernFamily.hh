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

#ifndef __ComputerModernFamily_hh__
#define __ComputerModernFamily_hh__

#include "Object.hh"
#include "SmartPtr.hh"
#include "String.hh"
#include "scaled.hh"
#include "MathVariant.hh"

class GMV_MathView_EXPORT ComputerModernFamily : public Object
{
protected:
  ComputerModernFamily(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&);
  virtual ~ComputerModernFamily();

public:
  static SmartPtr<ComputerModernFamily> create(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&);

  enum FontSizeId {
    FS_NIL = -1,
    FS_5,
    FS_6,
    FS_7,
    FS_8,
    FS_9,
    FS_10,
    FS_12,
    FS_17,
    FS_NOT_VALID
  };

  enum FontEncId {
    FE_NIL = -1,
    FE_CMR,
    FE_CMMI,
    FE_CMSY,
    FE_CMEX,
    FE_MSAM,
    FE_MSBM,
    FE_COMBINING,
    FE_H_STRETCHY,
    FE_V_STRETCHY,
    FE_SPECIAL_STRETCHY,
    FE_NOT_VALID
  };

  enum FontNameId {
    FN_NIL = -1,
    FN_CMR,
    FN_CMB,
    FN_CMBX,
    FN_CMBXTI,
    FN_CMTI,
    FN_CMSS,
    FN_CMSSI,
    FN_CMSSBX,
    FN_CMTT,
    FN_CMSY,
    FN_CMBSY,
    FN_CMMI,
    FN_CMMIB, 
    FN_CMEX,
    FN_MSAM,
    FN_MSBM,
    FN_NOT_VALID
  };

  static bool
  validFontNameId(FontNameId id)
  { return id > FN_NIL && id < FN_NOT_VALID; }

  static bool
  validFontEncId(FontEncId id)
  { return id > FE_NIL && id < FE_NOT_VALID; }

  static bool
  validFontSizeId(FontSizeId id)
  { return id > FS_NIL && id < FS_NOT_VALID; }

  static FontEncId encIdOfFontNameId(FontNameId);
  static FontSizeId fontSizeIdOfSize(int);
  static int sizeOfFontSizeId(FontSizeId);
  static const char* nameOfFontNameId(FontNameId);
  static String nameOfFont(FontNameId, FontSizeId);
  String nameOfFont(MathVariant, FontEncId, const scaled&) const;
  FontNameId findFont(MathVariant, FontEncId, scaled&, FontSizeId&) const;
  bool fontEnabled(FontNameId, FontSizeId = FS_10) const;

protected:
  FontNameId findBestFont(MathVariant, FontEncId, int, FontSizeId&) const;

private:
  bool configuredFont[FN_NOT_VALID][FS_NOT_VALID];
};

#endif // __ComputerModernFamily_hh__
