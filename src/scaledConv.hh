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

#ifndef __scaledConv_hh__
#define __scaledConv_hh__

#include "defs.h"
#include "scaled.hh"

scaled ToScaledPoints(const class Length&);

inline scaled mm2sp(float mm) { return scaled(mm * SP_PER_MM / SP_PER_PT); }
inline scaled cm2sp(float cm) { return scaled(cm * SP_PER_CM / SP_PER_PT); }
inline scaled pt2sp(float pt) { return scaled(pt * SP_PER_PT / SP_PER_PT); }
inline scaled pc2sp(float pc) { return scaled(pc * SP_PER_PC / SP_PER_PT); }
inline scaled in2sp(float in) { return scaled(in * SP_PER_IN / SP_PER_PT); }
inline scaled px2sp(float px) { return scaled(px * SP_PER_PX / SP_PER_PT); }
inline scaled px2sp(int px) { return scaled(px * SP_PER_PX / SP_PER_PT); }

inline float  sp2mm(const scaled& sp) { return sp.toFloat() * SP_PER_PT / SP_PER_MM; }
inline float  sp2cm(const scaled& sp) { return sp.toFloat() * SP_PER_PT / SP_PER_CM; }
inline float  sp2pt(const scaled& sp) { return sp.toFloat() * SP_PER_PT / SP_PER_PT; }
inline float  sp2pc(const scaled& sp) { return sp.toFloat() * SP_PER_PT / SP_PER_PC; }
inline float  sp2in(const scaled& sp) { return sp.toFloat() * SP_PER_PT / SP_PER_IN; }
inline float  sp2px(const scaled& sp) { return sp.toFloat() * SP_PER_PT / SP_PER_PX; }
inline int    sp2ipx(const scaled& sp) { return static_cast<int>(sp2px(sp)); }

#endif // __scaledConv_hh__

