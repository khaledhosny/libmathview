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

#ifndef __SparseMap_hh__
#define __SparseMap_hh__

#include "gmv_defines.h"

template <class T, int M, int N>
class GMV_MathView_EXPORT SparseMap
{
public:
  SparseMap(void)
  {
    for (unsigned i = 0; i < (1 << N); i++)
      defData[i] = T();
    for (unsigned i = 0; i < (1 << M); i++)
      data[i] = defData;
  }

  ~SparseMap()
  {
    for (unsigned i = 0; i < (1 << M); i++)
      {
	if (data[i] != defData) delete [] data[i];
	data[i] = 0;
      }
  }

protected:
  unsigned I(unsigned index) const
  { return index >> N; }

  unsigned J(unsigned index) const
  { return index & ((1 << N) - 1); }

public:
  void set(size_t index, const T& v)
  {
    unsigned i = I(index);
    assert(i < (1 << M));
    if (data[i] == defData)
      {
	data[i] = new T[1 << N];
	for (unsigned j = 0; j < (1 << N); j++)
	  data[i][j] = T();
      }
    data[i][J(index)] = v;
  }

  const T& operator[](size_t index) const
  { return data[I(index)][J(index)]; }

private:
  typedef T* TBlock;

  TBlock data[1 << M];
  T defData[1 << N];
};

#endif // __SparseMap_hh__
