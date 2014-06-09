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

#ifndef __SparseMap_hh__
#define __SparseMap_hh__

template <class T, int M, int N>
class SparseMap
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
