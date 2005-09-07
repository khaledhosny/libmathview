/* This file is part of MFD, a Math Formatting Device for various
 * contexts/backends 
 * 
 * Copyright (C) 2003 Luca Padovani <lpadovan@cs.unibo.it>,
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * For more information, please visit the project's home page
 * http://www.cs.unibo.it/~lpadovan/mfd/
 * or send an email to <lpadovan@cs.unibo.it>
 */

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

inline GMV_EXPORT bool
validMathVariant(MathVariant id)
{ return id >= NORMAL_VARIANT && id <= MONOSPACE_VARIANT; }

#endif // __MathVariant_hh__
