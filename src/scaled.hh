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

#ifndef scaled_hh
#define scaled_hh

#include <config.h>

#include "defs.h"

#define SP_EPSILON (float2sp(SCALED_POINTS_PER_PT * EPSILON))

inline int   truncFloat(float d) { return static_cast<int>(d); }
inline int   roundFloat(float d) { return static_cast<int>((d < 0) ? (d - 0.5) : (d + 0.5)); }
inline float floatMax(float d1, float d2) { return (d1 > d2) ? d1 : d2; }

#if defined(FLOATING_SCALED)

typedef float scaled;

inline int    sp2int(scaled sp) { return truncFloat(sp); }
inline float  sp2float(scaled sp) { return sp; }
inline scaled int2sp(int x) { return static_cast<float>(x); }
inline scaled float2sp(float x) { return x; }
inline bool   scaledLeq(scaled a, scaled b) { return a - b < SP_EPSILON; }
inline bool   scaledGeq(scaled a, scaled b) { return a - b > -SP_EPSILON; }
inline bool   scaledEq(scaled a, scaled b) { return scaledLeq(a, b) && scaledGeq(a, b); }

#elif defined(FIXED_SCALED)

typedef int scaled;

inline int    sp2int(scaled sp) { return sp; }
inline float  sp2float(scaled sp) { return static_cast<float>(sp); }
inline scaled int2sp(int x) { return x; }
inline scaled float2sp(float x) { return truncFloat(x); }
inline bool   scaledEq(scaled a, scaled b) { return a == b; }
inline bool   scaledLeq(scaled a, scaled b) { return a <= b; }
inline bool   scaledGeq(scaled a, scaled b) { return a >= b; }

#else
#error "No scaled type defined"
#endif

inline scaled scaledAbs(scaled x) { return (x < 0) ? -x : x; }
inline scaled scaledMin(scaled a, scaled b) { return (a < b) ? a : b; }
inline scaled scaledMax(scaled a, scaled b) { return (a > b) ? a : b; }
inline bool   scaledIn(scaled x, scaled a, scaled b) {
  return (scaledGeq(x, a) && scaledLeq(x, b));
}
inline scaled scaledProp(scaled x, scaled xMax, scaled normalize) {
  return float2sp((sp2float(x) * xMax) / normalize);
}

inline scaled mm2sp(float mm) { return float2sp(mm * SCALED_POINTS_PER_MM); }
inline scaled cm2sp(float cm) { return float2sp(cm * SCALED_POINTS_PER_CM); }
inline scaled pt2sp(float pt) { return float2sp(pt * SCALED_POINTS_PER_PT); }
inline scaled pc2sp(float pc) { return float2sp(pc * SCALED_POINTS_PER_PC); }
inline scaled in2sp(float in) { return float2sp(in * SCALED_POINTS_PER_IN); }
inline scaled px2sp(float px) { return float2sp(px * SCALED_POINTS_PER_PX); }
inline scaled px2sp(int px) { return float2sp(px * SCALED_POINTS_PER_PX); }

inline float  sp2mm(scaled sp) { return sp2float(sp) / SCALED_POINTS_PER_MM; }
inline float  sp2cm(scaled sp) { return sp2float(sp) / SCALED_POINTS_PER_CM; }
inline float  sp2pt(scaled sp) { return sp2float(sp) / SCALED_POINTS_PER_PT; }
inline float  sp2pc(scaled sp) { return sp2float(sp) / SCALED_POINTS_PER_PC; }
inline float  sp2in(scaled sp) { return sp2float(sp) / SCALED_POINTS_PER_IN; }
inline float  sp2px(scaled sp) { return sp2float(sp) / SCALED_POINTS_PER_PX; }
inline int    sp2ipx(scaled sp) { return truncFloat(sp2px(sp)); }

#endif // scaled_hh

