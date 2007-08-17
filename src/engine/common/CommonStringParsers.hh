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

#ifndef __CommonStringParsers_hh__
#define __CommonStringParsers_hh__

#include "TemplateStringParsers.hh"

typedef Union< Set<T_BLACK,T_SILVER,T_GRAY,T_WHITE,T_MAROON,T_RED,T_PURPLE,T_FUCHSIA>,
	       Set<T_GREEN,T_LIME,T_OLIVE,T_YELLOW,T_NAVY,T_BLUE,T_TEAL,T_AQUA> > TokenSet_HTMLColor;

typedef Parse<ScanToken,bool> ParseBoolean;
typedef Parse<ScanUnsignedInteger,int> ParseUnsignedInteger;
typedef Parse<ScanInteger,int> ParseInteger;
typedef ParseChoice< ParseRGBColor,ParseTokenSet<TokenSet_HTMLColor> > ParseColor;
typedef Parse<ScanUnsignedNumber,float> ParseUnsignedNumber;
typedef Parse<ScanNumber,float> ParseNumber;
typedef ParseTokenSet<TokenSet_Unit> ParseUnit;

#endif // __CommonStringParsers_hh__
