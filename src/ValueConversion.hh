// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef ValueConversion_hh
#define ValueConversion_hh_hh

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "AFont.hh"
#include "Value.hh"

RowAlignId      ToRowAlignId(const Value*);
ColumnAlignId   ToColumnAlignId(const Value*);
GroupAlignId 	ToGroupAlignId(const Value*);
TableLineId  	ToLineId(const Value*);
MathSpaceId    	ToNamedSpaceId(const Value*);
UnitId         	ToUnitId(const Value*);
FontWeightId 	ToFontWeightId(const Value*);
FontStyleId  	ToFontStyleId(const Value*);
RGBValue      	ToRGB(const Value*);
OperatorFormId  ToFormId(const Value*);
FractionAlignId ToFractionAlignId(const Value*);

#endif
