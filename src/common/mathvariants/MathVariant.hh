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

#ifndef __MathVariant_hh__
#define __MathVariant_hh__

#include "gmv_defines.h"

enum MathVariant
  {
    NORMAL_VARIANT,
    BOLD_VARIANT,
    ITALIC_VARIANT,
    BOLD_ITALIC_VARIANT,
    DOUBLE_STRUCK_VARIANT,
    BOLD_FRAKTUR_VARIANT,
    SCRIPT_VARIANT,
    BOLD_SCRIPT_VARIANT,
    FRAKTUR_VARIANT,
    SANS_SERIF_VARIANT,
    BOLD_SANS_SERIF_VARIANT,
    SANS_SERIF_ITALIC_VARIANT,
    SANS_SERIF_BOLD_ITALIC_VARIANT,
    MONOSPACE_VARIANT
  };

inline GMV_MathView_EXPORT bool
validMathVariant(MathVariant id)
{ return id >= NORMAL_VARIANT && id <= MONOSPACE_VARIANT; }

#endif // __MathVariant_hh__
