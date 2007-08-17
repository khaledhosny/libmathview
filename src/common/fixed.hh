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

#ifndef __fixed_hh__
#define __fixed_hh__

#include <limits>

#include "gmv_defines.h"

template<typename T = int, int precision = std::numeric_limits<T>::digits / 2>
GMV_MathView_EXPORT class fixed
{
public:
  fixed(void) : value(0) { }
  fixed(T v, bool) : value(v) { }
  fixed(T v) : value(static_cast<T>(v) << precision) { }
  fixed(float v) : value(static_cast<T>(v * (static_cast<T>(1) << precision))) { }
  fixed(double v) : value(static_cast<T>(v * (static_cast<T>(1) << precision))) { }

  static fixed zero(void) { return fixed(0, true); }
  static fixed half(void) { return fixed(static_cast<T>(1) << (precision - 1), true); }
  static fixed one(void) { return fixed(static_cast<T>(1) << precision, true); }
  static fixed min(void) { return fixed(std::numeric_limits<T>::min(), true); }
  static fixed max(void) { return fixed(std::numeric_limits<T>::max(), true); }
  
  template<typename S, int p> friend fixed<S,p> abs(const fixed<S,p>&);
  template<typename S, int p> friend fixed<S,p> trunc(const fixed<S,p>&);
  template<typename S, int p> friend fixed<S,p> round(const fixed<S,p>&);
  template<typename S, int p> friend fixed<S,p> ceil(const fixed<S,p>&);
  template<typename S, int p> friend fixed<S,p> floor(const fixed<S,p>&);

  template<typename S, int p> friend fixed<S,p> operator+(const fixed<S,p>&, const fixed<S,p>&);
  template<typename S, int p> friend fixed<S,p> operator-(const fixed<S,p>&, const fixed<S,p>&);
  template<typename S, int p> friend fixed<S,p> operator-(const fixed<S,p>&);
  template<typename S, int p> friend fixed<S,p> operator*(const fixed<S,p>&, const fixed<S,p>&);
  template<typename S, int p> friend fixed<S,p> operator*(const fixed<S,p>&, int);
  template<typename S, int p> friend fixed<S,p> operator*(const fixed<S,p>&, float);
  template<typename S, int p> friend fixed<S,p> operator*(const fixed<S,p>&, double);
  template<typename S, int p> friend fixed<S,p> operator*(int, const fixed<S,p>&);
  template<typename S, int p> friend fixed<S,p> operator*(float, const fixed<S,p>&);
  template<typename S, int p> friend fixed<S,p> operator*(double, const fixed<S,p>&);
  template<typename S, int p> friend fixed<S,p> operator/(const fixed<S,p>&, int);
  template<typename S, int p> friend fixed<S,p> operator/(const fixed<S,p>&, float);
  template<typename S, int p> friend fixed<S,p> operator/(const fixed<S,p>&, double);
  template<typename S, int p> friend fixed<S,p>& operator+=(fixed<S,p>&, const fixed<S,p>&);
  template<typename S, int p> friend fixed<S,p>& operator-=(fixed<S,p>&, const fixed<S,p>&);
  template<typename S, int p> friend fixed<S,p>& operator*=(fixed<S,p>&, int);
  template<typename S, int p> friend fixed<S,p>& operator*=(fixed<S,p>&, float);
  template<typename S, int p> friend fixed<S,p>& operator*=(fixed<S,p>&, double);
  template<typename S, int p> friend fixed<S,p>& operator/=(fixed<S,p>&, int);
  template<typename S, int p> friend fixed<S,p>& operator/=(fixed<S,p>&, float);
  template<typename S, int p> friend fixed<S,p>& operator/=(fixed<S,p>&, double);
  template<typename S, int p> friend bool operator==(const fixed<S,p>&, const fixed<S,p>&);
  template<typename S, int p> friend bool operator!=(const fixed<S,p>&, const fixed<S,p>&);
  template<typename S, int p> friend bool operator<(const fixed<S,p>&, const fixed<S,p>&);
  template<typename S, int p> friend bool operator>(const fixed<S,p>&, const fixed<S,p>&);
  template<typename S, int p> friend bool operator<=(const fixed<S,p>&, const fixed<S,p>&);
  template<typename S, int p> friend bool operator>=(const fixed<S,p>&, const fixed<S,p>&);

  static int getPrecision(void) { return precision; }
  int toInt(void) const { return value >> precision; }
  float toFloat(void) const { return static_cast<float>(value) / fixed::one().value; }
  double toDouble(void) const { return static_cast<double>(value) / fixed::one().value; }
  T getValue(void) const { return value; }
  
private:
  T value;
};

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
abs(const fixed<T,p>& f)
{ 
  return fixed<T,p>((f.value > 0) ? f.value : -f.value, true);
}

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
trunc(const fixed<T,p>& f)
{
  if (f.value > 0) return fixed<T,p>(f.value & ~(fixed<T,p>::one().value - 1), true);
  else return fixed<T,p>(-(-f.value & ~(fixed<T,p>::one().value - 1)), true);
}

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
round(const fixed<T,p>& f)
{
  if (f.value > 0) return trunc(f + fixed<T,p>::half());
  else return trunc(f - fixed<T,p>::half());
}

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
ceil(const fixed<T,p>& f)
{
  if (f.value > 0) return round(f + fixed<T,p>::half());
  else return trunc(f);
}

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
floor(const fixed<T,p>& f)
{
  if (f.value > 0) return trunc(f);
  else return round(f - fixed<T,p>::half());
}

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
operator+(const fixed<T,p>& f1, const fixed<T,p>& f2)
{ return fixed<T,p>(f1.value + f2.value, true); }

template <typename T, int p> 
inline GMV_MathView_EXPORT fixed<T,p>
operator-(const fixed<T,p>& f1, const fixed<T,p>& f2)
{ return fixed<T,p>(f1.value - f2.value, true); }

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
operator-(const fixed<T,p>& f)
{ return fixed<T,p>(-f.value, true); }

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
operator*(const fixed<T,p>& f1, const fixed<T,p>& f2) 
{ 
  T a = f1.value >> (p / 2);
  T a1 = f1.value & ((1 << (p / 2)) - 1);
  T b = f2.value >> (p / 2);
  T b1 = f2.value & ((1 << (p / 2)) - 1);
  return fixed<T,p>(static_cast<T>(a * b + ((a1 * b) >> (p / 2)) + ((a * b1) >> (p / 2)) + ((a1 * b1) >> p)), true);
}

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
operator*(const fixed<T,p>& f, int v) 
{ return fixed<T,p>(f.value * v, true); }

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
operator*(const fixed<T,p>& f, float v)
{ return fixed<T,p>(static_cast<T>(f.value * v), true); }

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
operator*(const fixed<T,p>& f, double v) 
{ return fixed<T,p>(static_cast<T>(f.value * v), true); }

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
operator/(const fixed<T,p>& f, int v)
{ return fixed<T,p>(f.value / v, true); }

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
operator/(const fixed<T,p>& f, float v)
{ return fixed<T,p>(static_cast<T>(f.value / v), true); }

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
operator/(const fixed<T,p>& f, double v)
{ return fixed<T,p>(static_cast<T>(f.value / v), true); }

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
operator*(int v, const fixed<T,p>& f) 
{ return fixed<T,p>(f.value * v, true); }

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
operator*(float v, const fixed<T,p>& f)
{ return fixed<T,p>(static_cast<T>(f.value * v), true); }

template <typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>
operator*(double v, const fixed<T,p>& f) 
{ return fixed<T,p>(static_cast<T>(f.value * v), true); }

template<typename S, int p> 
inline GMV_MathView_EXPORT fixed<S,p>&
operator+=(fixed<S,p>& f1, const fixed<S,p>& f2)
{
  f1.value += f2.value;
  return f1;
}

template<typename S, int p> 
inline GMV_MathView_EXPORT fixed<S,p>&
operator-=(fixed<S,p>& f1, const fixed<S,p>& f2)
{
  f1.value -= f2.value;
  return f1;
}

template<typename S, int p>
inline GMV_MathView_EXPORT fixed<S,p>&
operator*=(fixed<S,p>& f, int v)
{
  f.value *= v;
  return f;
}

template<typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>&
operator*=(fixed<T,p>& f, float v)
{
  f.value = static_cast<T>(f.value * v);
  return f;
}

template<typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>&
operator*=(fixed<T,p>& f, double v)
{
  f.value = static_cast<T>(f.value * v);
  return f;
}

template<typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>&
operator/=(const fixed<T,p>& f, int v)
{
  f.value /= v;
  return f;
}

template<typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>&
operator/=(const fixed<T,p>& f, float v)
{
  f.value = static_cast<T>(f.value / v);
  return f;
}

template<typename T, int p>
inline GMV_MathView_EXPORT fixed<T,p>&
operator/=(const fixed<T,p>& f, double v)
{
  f.value = static_cast<T>(f.value / v);
  return f;
}

template <typename T, int p>
inline GMV_MathView_EXPORT bool
operator==(const fixed<T,p>& f1, const fixed<T,p>& f2)
{ return f1.value == f2.value; }

template <typename T, int p>
inline GMV_MathView_EXPORT bool
operator!=(const fixed<T,p>& f1, const fixed<T,p>& f2)
{ return f1.value != f2.value; }

template <typename T, int p>
inline GMV_MathView_EXPORT bool
operator<(const fixed<T,p>& f1, const fixed<T,p>& f2)
{ return f1.value < f2.value; }

template <typename T, int p>
inline GMV_MathView_EXPORT bool
operator>(const fixed<T,p>& f1, const fixed<T,p>& f2)
{ return f1.value > f2.value; }

template <typename T, int p>
inline GMV_MathView_EXPORT bool
operator<=(const fixed<T,p>& f1, const fixed<T,p>& f2)
{ return f1.value <= f2.value; }

template <typename T, int p>
inline GMV_MathView_EXPORT bool
operator>=(const fixed<T,p>& f1, const fixed<T,p>& f2)
{ return f1.value >= f2.value; }

#endif // __fixed_hh__

