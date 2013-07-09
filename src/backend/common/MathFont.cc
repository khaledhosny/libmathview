// Copyright (C) 2013, Khaled Hosny <khaledhosny@eglug.org>.
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

#include <config.h>

#include "MathFont.hh"

MathFont::MathFont()
  : tableData(0)
  , unitsPerEM(0)
{ }

MathFont::~MathFont()
{
  delete [] tableData;
}

SmartPtr<MathFont>
MathFont::create()
{
  return new MathFont();
}

int
MathFont::getConstant(MathConstant constant) const
{
  int value = 0;
  if (tableData)
    {
      const uint16_t* constants = (const uint16_t*)(tableData + SWAP(((const MathTableHeader*)tableData)->mathConstants));
      if (constant < firstMathValueRecord)
        {
          /* it's a simple 16-bit value */
          value =  SWAP(constants[constant]);
        }
      else if (constant <= lastMathValueRecord)
        {
          const MathValueRecord* valueRecords = (const MathValueRecord*)
              ((char*)constants + firstMathValueRecord * sizeof(uint16_t) - firstMathValueRecord * sizeof(MathValueRecord));
          value =  SWAP(valueRecords[constant].value);
        }
      else if (constant <= lastMathConstant)
        {
          value =  SWAP(constants[constant + (lastMathValueRecord - firstMathValueRecord + 1)]);
        }
    }

  return value;
}
